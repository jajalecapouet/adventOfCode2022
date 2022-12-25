#include <iostream>
#include <fstream>
#include <cmath>
#include <ctype.h>
#include <unistd.h>

long long	parseLine(const std::string &line)
{
	int exp = 0;
	long long nb = 0;
	//std::cout << "SNAFU line = " << line << " = ";
	std::string::size_type	start = 0;
	std::string::size_type	end;
	while (start < line.size() && isspace(line[start]))
		++start;
	end = start + 1;
	while (end < line.size() && !isspace(line[end]))
		++end;
	for (std::string::size_type idx = end; idx >= start + 1; --idx)
	{
		switch (line[idx - 1])
		{
			case '0':
				break;
			case '1':
				nb += pow(5, exp);
				break;
			case '2':
				nb += 2 * pow(5, exp);
				break;
			case '-':
				nb -= pow(5, exp);
				break;
			case '=':
				nb -= 2 * pow(5, exp);
				break;
			default:
	//			std::cout << "0\n";
				return 0;
		}
		++exp;
	}
	//std::cout << nb << '\n';
	return nb;
}

int	lgBase5(long long n)
{
	int	i = 0;
	while (n)
	{
		n /= 5;
		++i;
	}
	return i;
}

std::string	snafuMan(long long n) {
	bool sign = n < 0;
	if (sign)
		n = -n;
	std::string snafu;
	int exp = lgBase5(n) + 1;

	while (n)
	{
		//usleep(250000);
		--exp;
		//std::cout << exp << ' ' << pow(5, exp) << ' ' << n << ' ' << snafu << '\n';
		long long toCmp2 = 2 * pow(5, exp);
		long long toCmp = pow(5, exp);
		if (n > 0) {
			if (n > 3 * toCmp / 2)
			{
				n -= toCmp2;
				snafu.insert(snafu.end(), '2');
			}
			else if (n > toCmp / 2)
			{
				n -= toCmp;
				snafu.insert(snafu.end(), '1');
			}
			else
				snafu.insert(snafu.end(), '0');
		}
		else
		{
			if (-n > 3 * toCmp / 2)
			{
				n += toCmp2;
				snafu.insert(snafu.end(), '=');
			}
			else if (-n > toCmp / 2)
			{
				n += toCmp;
				snafu.insert(snafu.end(), '-');
			}
			else
				snafu.insert(snafu.end(), '0');
		}
	}
	while (exp)
	{
		snafu.insert(snafu.end(), '0');
		--exp;
	}
	if (snafu.front() == '0') snafu.erase(snafu.begin());
	if (sign)
		for (std::string::size_type i = 0; i != snafu.size() - 1; ++i)
		{
			switch (snafu[i])
			{
				case '1':
					snafu[i] = '-';
					break;
				case '2':
					snafu[i] = '=';
					break;
				case '-':
					snafu[i] = '1';
					break;
				case '=':
					snafu[i] = '2';
					break;
				default:
					break;
			}
		}
	return snafu;
}

int main(int ac, char **av)
{
	if (ac != 2) return 1;
	std::ifstream input(av[1]);
	if (input.fail()) return 2;

	std::string line;
	long long sum = 0;
	while (!input.eof())
	{
		getline(input,line);
		if (line.back() == 13) line.erase(--line.end());
		sum += parseLine(line);
	}
	std::cout << "sum found is : " << sum << '\n';
	std::string snafu = snafuMan(sum);
	std::cout << "the SNAFU of this sum is " << snafu << '\n';
	return 0;
}