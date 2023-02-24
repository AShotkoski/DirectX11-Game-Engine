#pragma once
#include <random>

// Helper class for making the generation of random numbers and normal distributions less
// annoying. Note that these function are slow since they create a rng device each evocation
// and it would be a good idea to create non static member functions that share a mt19937
// object if this factory is ever needed to be used when performance matters.


class NumberFactory
{
public:
	template <typename T = float>
	static T RandomReal( T min, T max )
	{
		using namespace std;

		mt19937 rng( random_device{}( ) );

		uniform_real_distribution<T> dist( min, max );
		return dist( rng );
	}
	template <typename T>
	static T RandomInt( T min, T max )
	{
		using namespace std;

		mt19937 rng( random_device{}( ) );

		uniform_int_distribution<T> dist( min, max );
		return dist( rng );
	}
	template <typename T>
	static T NormalReal( T mean, T sigma, T min, T max )
	{
		using namespace std;

		mt19937 rng( random_device{}( ) );

		normal_distribution<T> dist( mean, sigma );
		T clamped = std::clamp( dist( rng ), min, max );
		return clamped;
	}
};