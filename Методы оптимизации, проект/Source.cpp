#include <iostream>

#include "simplex.hpp"

#define BOOST_TEST_MODULE SimplexTest
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test1)
{
	diet_solver s("input1.txt");
	BOOST_CHECK_EQUAL(true, s.solve());
	auto res = s.get_result();
	BOOST_CHECK_CLOSE(8.0 / 3, res[0], 0.0001);
	BOOST_CHECK_CLOSE(11.0 / 3, res[1], 0.0001);
	BOOST_CHECK_CLOSE(68.0 / 3, s.get_price(), 0.001);
	std::cout << "Test1: \n";
	std::cout << "Optimal solution: (";
	std::cout << res[0];
	for (auto it = res.begin() + 1; it != res.end(); ++it)
	{
		std::cout << ", " << *it;
	}
	std::cout << ")" << std::endl;
	std::cout << "Objective value: " << s.get_price() << std::endl;
	std::cout << "Objective value: " << s.get_price() << std::endl;
}

BOOST_AUTO_TEST_CASE(test2)
{
	diet_solver s("input2.txt");
	BOOST_CHECK_EQUAL(true, s.solve());
	auto res = s.get_result();
	BOOST_CHECK_CLOSE(3, res[0], 0.0001);
	BOOST_CHECK_CLOSE(0.5, res[1], 0.0001);
	BOOST_CHECK_CLOSE(11, s.get_price(), 0.001);
	std::cout << "Test2: \n";
	std::cout << "Optimal solution: (";
	std::cout << res[0];
	for (auto it = res.begin() + 1; it != res.end(); ++it)
	{
		std::cout << ", " << *it;
	}
	std::cout << ")" << std::endl;
	std::cout << "Objective value: " << s.get_price() << std::endl;
}

BOOST_AUTO_TEST_CASE(test3)
{
	diet_solver s("input3.txt");
	BOOST_CHECK_EQUAL(true, s.solve());
	auto res = s.get_result();
	BOOST_CHECK_CLOSE(0.35433, res[0], 0.01);
	BOOST_CHECK_CLOSE(4.9215, res[1], 0.01);
	BOOST_CHECK_CLOSE(0, res[2], 0.0001);
	BOOST_CHECK_CLOSE(124.17, s.get_price(), 0.01);
	std::cout << "Test3: \n";
	std::cout << "Optimal solution: (";
	std::cout << res[0];
	for (auto it = res.begin() + 1; it != res.end(); ++it)
	{
		std::cout << ", " << *it;
	}
	std::cout << ")" << std::endl;
	std::cout << "Objective value: " << s.get_price() << std::endl;
	system("pause");
}