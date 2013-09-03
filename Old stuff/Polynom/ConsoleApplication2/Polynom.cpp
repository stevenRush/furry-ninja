#include "stdafx.h"
#include "Polynom.h"


CPolynom::CPolynom(void) :
	m_Degree(0)
{
	
}

// Coefs[i] - это коэффициент при x^i
CPolynom::CPolynom(double Coefs[], int n) :
	m_Degree(n)
{
	for(size_t i=0; i <= n; ++i)
		m_Coefs.push_back(Coefs[n-i]);
	CheckNulls();
}

void CPolynom::CheckNulls()
{
	while(std::fabs(m_Coefs[m_Degree]) < 1e-10 && m_Degree > 0)
		--m_Degree;
}

// готово
double& CPolynom::operator[](const size_t idx) 
{
	return m_Coefs[idx];
}

// готово
double CPolynom::operator[](const size_t idx) const
{
	return m_Coefs[idx];
}

// готово
CPolynom CPolynom::raw_Add(const CPolynom& lv, const CPolynom& rv)
{
	if (lv.m_Degree < rv.m_Degree)
		return raw_Add(rv, lv);
	CPolynom res = lv;
	for(size_t i=0; i <= rv.m_Degree; ++i)
		res[i] += rv[i];
	return res;
}

// готово
CPolynom CPolynom::raw_Sub(const CPolynom& lv, const CPolynom& rv)
{
	if (lv.m_Degree < rv.m_Degree)
		return raw_Sub(rv, lv) * -1;
	
	CPolynom res = lv;
	for(size_t i=0; i <= rv.m_Degree; ++i)
		res[i] -= rv[i];
	res.CheckNulls();
	return res;
}

// готово
CPolynom CPolynom::raw_Mul(const CPolynom& v, const double& num)
{
	CPolynom res = v;
	for(size_t i=0; i <= res.m_Degree; ++i)
		res[i] *= num;
	res.CheckNulls();
	return res;
}

// готово
CPolynom CPolynom::raw_Mul(const CPolynom& lv, const CPolynom& rv)
{
	if (lv.m_Degree < rv.m_Degree) 
		return raw_Mul(rv, lv);
	CPolynom res;
	int coef = 0;
	res.m_Degree = lv.m_Degree + rv.m_Degree;
	for(size_t i=0; i <= res.m_Degree; ++i)
	{
		coef = 0;
		for(size_t j = i > lv.m_Degree ? i-lv.m_Degree : 0; j <= rv.m_Degree && j <= i; ++j)
			coef += rv[j] * lv[i-j];
		res.m_Coefs.push_back(coef);
	}
	return res;
}

// готово
void CPolynom::raw_Div(const CPolynom& lv, const CPolynom& rv, CPolynom& ch, CPolynom& ost)
{
	ost = lv;
	ch.m_Degree = lv.m_Degree - rv.m_Degree;
	while(ost.m_Degree >= rv.m_Degree && !ost.isEmpty())
	{
		CPolynom temp = rv;
		temp.Shift(ost.m_Degree-rv.m_Degree);
		double coef = ost[ost.m_Degree] / rv[rv.m_Degree];
		temp *= coef;
		ch.m_Coefs.insert(ch.m_Coefs.begin(), coef);
		ost = ost - temp;
	}
	for(size_t i=0, c = ch.m_Degree - ch.m_Coefs.size() + 1; i < c; ++i)
		ch.m_Coefs.insert(ch.m_Coefs.begin(), 0);
}

// готово
void CPolynom::Shift(size_t c)
{
	std::vector<double> temp(c, 0);
	for(size_t i=0; i < m_Coefs.size(); ++i)
		temp.push_back(m_Coefs[i]);
	m_Coefs = temp;
	m_Degree += c;
}

// готово
bool CPolynom::isEmpty()
{
	return m_Degree == 0 && std::fabs(m_Coefs[0]) < 1e-10 ;
}

// готово
CPolynom CPolynom::operator+(const CPolynom& rv) const
{
	return raw_Add(*this, rv);
}

// готово
CPolynom CPolynom::operator-(const CPolynom& rv) const
{
	return raw_Sub(*this, rv);
}

// готово
CPolynom CPolynom::operator*(const CPolynom& rv) const
{
	return raw_Mul(*this, rv);
}

// готово
CPolynom CPolynom::operator*(const double& rv) const
{
	return raw_Mul(*this, rv);
}

// готово
void CPolynom::operator*=(const double& rv)
{
	*this = raw_Mul(*this, rv);
}

// готово
void CPolynom::operator*=(const CPolynom& rv)
{
	*this = raw_Mul(*this, rv);
}

// готово
CPolynom CPolynom::operator/(const CPolynom& rv) const
{
	CPolynom ch, ost;
	raw_Div(*this, rv, ch, ost);
	return ch;
}

// готово
CPolynom CPolynom::operator%(const CPolynom& rv) const
{
	CPolynom ch, ost;
	raw_Div(*this, rv, ch, ost);
	return ost;
}

// готово
void CPolynom::Divide(const CPolynom& lv, const CPolynom& rv, CPolynom& ch, CPolynom& ost)
{
	raw_Div(lv, rv, ch, ost);
}

// готово
CPolynom CPolynom::NOD(const CPolynom& lv, const CPolynom& rv)
{
	CPolynom q = lv, r = rv;
	while(!r.isEmpty())
	{
		CPolynom t = r;
		r = q % r;
		q = t;		
	}
	return q;
}

// готово
CPolynom CPolynom::Derivative() const
{
	CPolynom res;
	res.m_Degree = this->m_Degree-1;
	for(size_t i=0; i < this->m_Degree; ++i)
		res.m_Coefs.push_back(this->m_Coefs[i+1] * (i+1));
	return res;
}

// готово
double CPolynom::ValueAt(double x) const
{
	double res = 0;
	double pow = 1;
	for(size_t i=0; i <= m_Degree; ++i)
	{
		res += pow * m_Coefs[i];
		pow *= x;
	}
	return res;
}

// готово
int CPolynom::CheckRoot(double x) const
{
	int res = 0;
	CPolynom d = *this;
	while(fabs(d.ValueAt(x)) < 1e-10)
	{
		res++;
		d = d.Derivative();
	}
	return res;
}

// готово
bool CPolynom::IsConstant() const
{
	return m_Degree == 0;
}

// вроде работает
void CPolynom::Decomposition() const
{
	int free;
	int i;
	int root = 0;
	bool found;
	CPolynom t = *this;
	while (!t.IsConstant())
	{
		i = -1;
		root = 0;
		free = t[0];
		found = false;
		while(i <= std::sqrt(std::abs(free)))
		{
			++i;
			if (free != 0 && (free != 0 && i == 0 || free % i != 0))
				continue;
			if (IsNull(t.ValueAt(i)))
			{
				root = i;
				found = true;
				break;
			}
			if (IsNull(t.ValueAt(i * -1)))
			{
				root = i * -1;
				found = true;
				break;
			}
			if (IsNull(t.ValueAt(free / i)))
			{
				root = free / i;
				found = true;
				break;
			}
			if (IsNull(t.ValueAt((free / i) * -1)))
			{
				root = (free / i) * -1;
				found = true;
				break;
			}
		}
		std::cout << "(";
		if (!found) 
		{
			t.PrintNoNewLine();
			std::cout << ")" << std::endl;
			return;
		}
		else
		{
			double arr[] = {1, -root};
			CPolynom d(arr, 1);
			d.PrintNoNewLine();
			t = t / CPolynom(arr, 1);
		}
		std::cout << ")";
	}
	std::cout << std::endl;
}

void CPolynom::PrintNoNewLine()
{
	for(size_t i=m_Degree+1; i > 0; --i)
	{
		if (i == m_Degree+1)
		{
			std::cout << m_Coefs[i-1] << "x" << i-1 << " ";
			continue;
		}
		if (m_Coefs[i-1] >= 0)
			std::cout << "+ " << m_Coefs[i-1];
		else
			std::cout << "- " << std::fabs(m_Coefs[i-1]);
		if (i > 1)
			std::cout << "x" << i-1 << " ";
	}
}

void CPolynom::Print()
{
	PrintNoNewLine();
	std::cout << std::endl;
}

CPolynom::~CPolynom(void)
{

}
