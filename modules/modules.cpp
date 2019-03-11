
import std.core;

int main ()
{
	const auto vector = std::vector<std::string> { "$", "#", "~", "&" };

	const auto print = [] (const std::string& str)
	{
		std::cout << str << ' ';
	};

	std::for_each(vector.begin(), vector.end(), print);

	return 0;
}
