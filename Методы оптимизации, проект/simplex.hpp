#include <vector>
#include <unordered_set>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

class simplex_solver
{
public:
	typedef std::vector<std::vector<double>> vector2D;
	typedef std::vector<double> vector1D;

	enum solving_result
	{
		srFound,
		srUnbounded
	};
	
	simplex_solver() = default;

	void init(const vector2D & _A, const vector1D & _b, const vector1D & _c)
	{
		c = _c;
		non_basis_size = _c.size();
		whole_size = non_basis_size + _b.size();
		c.resize(whole_size);
		a.resize(whole_size);

		for (size_t index = non_basis_size; index < whole_size; ++index)
		{
			a[index] = _A[index - non_basis_size];
		}

		b.resize(whole_size);
		for (size_t index = non_basis_size; index < whole_size; ++index)
		{
			b[index] = _b[index - non_basis_size];
		}

		for (auto & row : a)
		{
			row.resize(whole_size);
		}

		for (size_t index = 0; index < non_basis_size; ++index)
		{
			nonbasis.insert(index);
		}

		for (size_t index = non_basis_size; index < whole_size; ++index)
		{
			basis.insert(index);
		}

		v = 0;
	}

	solving_result solve()
	{
		size_t pivot = _get_pivot();
		while (c[pivot] > 0)
		{
			double delta = std::numeric_limits<double>::infinity();
			size_t min_index = -1;
			for (size_t index : basis)
			{
				if (a[index][pivot] > 0)
				{
					if (delta > b[index] / a[index][pivot])
					{
						delta = b[index] / a[index][pivot];
						min_index = index;
					}
				}
				else
				{
					delta = std::min(delta, std::numeric_limits<double>::infinity());
				}
			}

			if (delta == std::numeric_limits<double>::infinity())
			{
				return solving_result::srUnbounded;
			}
			_change_basis(min_index, pivot);
			pivot = _get_pivot();
		}

		result.resize(non_basis_size);
		for (size_t index = 0; index < non_basis_size; ++index)
		{
			if (basis.find(index) != basis.end())
			{
				result[index] = b[index];
			}
		}
		return solving_result::srFound;
	}

	vector1D get_result() const
	{
		return result;
	}

	vector1D get_dual_solution() const
	{
		vector1D result(whole_size - non_basis_size);
		for (size_t index = non_basis_size; index < whole_size; ++index)
		{
			if (nonbasis.find(index) != nonbasis.end())
			{
				result[index - non_basis_size] = -c[index];
			}
			else
			{
				result[index - non_basis_size] = 0;
			}
		}
		return result;
	}

	double get_objective_value() const
	{
		return v;
	}

private:
	vector1D c;
	vector2D a;
	vector1D b;
	std::unordered_set<size_t> basis;
	std::unordered_set<size_t> nonbasis;
	size_t non_basis_size;
	size_t whole_size;
	vector1D result;
	double v;

	// min_index = l; pivot = e
	void _change_basis(size_t min_index, size_t pivot)
	{
		b[pivot] = b[min_index] / a[min_index][pivot];

		nonbasis.erase(pivot);
		basis.erase(min_index);

		//std::vector<std::vector<double>> temp_a = a;

		for (size_t index : nonbasis)
		{
			a[pivot][index] = a[min_index][index] / a[min_index][pivot];
		}
		a[pivot][min_index] = 1 / a[min_index][pivot];

		for (size_t index : basis)
		{
			b[index] -= a[index][pivot] * b[pivot];

			for (size_t nbasis_index : nonbasis)
			{
				a[index][nbasis_index] -= a[index][pivot] * a[pivot][nbasis_index];
			}
			a[index][min_index] = -a[index][pivot] * a[pivot][min_index];
		}

		v += c[pivot] * b[pivot];
		for (size_t index : nonbasis)
		{
			c[index] -= c[pivot] * a[pivot][index];
		}
		c[min_index] = -c[pivot] * a[pivot][min_index];

		for (auto & row : a)
		{
			row[pivot] = 0;
		}
		a[min_index].assign(whole_size, 0);
		b[min_index] = 0;
		c[pivot] = 0;
		nonbasis.insert(min_index);
		basis.insert(pivot);
	}

	size_t _get_pivot() const
	{
		size_t max_index = 0;
		for (size_t index = 1; index < c.size(); ++index)
		{
			if (c[max_index] < c[index])
			{
				max_index = index;
			}
		}
		return max_index;
	}
};

class diet_solver
{
private:
	typedef std::vector<std::vector<double>> vector2D;
	typedef std::vector<double> vector1D;

public:
	diet_solver(const std::string & file_name) :
		stream(file_name)
	{
		// reading product count
		_next_line();
		size_t product_count = _next<size_t>();

		vector1D costs(product_count);
		vector2D constraint_matrix;
		// reading products' cost
		_next_line();
		for (size_t index = 0; index < product_count; ++index)
		{
			costs[index] = _next<double>();
		}

		_next_line();
		size_t nutrients_count = _next<size_t>();

		vector1D temp_vec(2 * nutrients_count);
		constraint_matrix.assign(product_count, temp_vec);

		for (size_t index = 0; index < product_count; ++index)
		{
			_next_line();
			for (size_t nutr_index = 0; nutr_index < nutrients_count; ++nutr_index)
			{
				double value = _next<double>();
				constraint_matrix[index][nutr_index] = value;
				constraint_matrix[index][nutr_index + nutrients_count] = -value;
			}
		}

		vector1D bounds(2 * nutrients_count);
		_next_line();
		for (size_t index = 0; index < nutrients_count; ++index)
		{
			bounds[index] = _next<double>();
		}

		_next_line();
		for (size_t index = nutrients_count; index < 2 * nutrients_count; ++index)
		{
			bounds[index] = -_next<double>();
		}

		solver.init(constraint_matrix, costs, bounds);
	}

	bool solve()
	{
		return solver.solve() == simplex_solver::srFound;
	}

	vector1D get_result() const
	{
		return solver.get_dual_solution();
	}

	double get_price() const
	{
		return solver.get_objective_value();
	}

private:
	std::ifstream stream;
	std::istringstream s;

	simplex_solver solver;

	void _next_line()
	{
		std::string st;
		std::getline(stream, st);
		while (st[0] == '#')
		{
			std::getline(stream, st);
		}
		s = std::istringstream(st);
	}

	template<class T>
	T _next()
	{
		T value;
		s >> value;
		return value;
	}
};