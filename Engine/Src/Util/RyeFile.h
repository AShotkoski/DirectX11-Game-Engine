#pragma once

//
// This is a simple implementation of a file parser that can store and read different 
// parameters or settings that might need to be used in a game class.
// It is designed to be used in a way where each RyeFile object represents one file.
// Each parameter is represented by its own object, which
//

//remove all these
#include "log.h"
#include <filesystem>
#include <fstream>

class RyeParam
{
public:
	template<typename T>
		requires std::is_fundamental<T>::value
	RyeParam( const std::string& key, const T& data )
	{
		//todo check bool and change to str

		data_.push_back( key );
		data_.push_back( std::to_string( data ) );		
	}
	RyeParam( std::vector<std::string> data )
	{
		// validate data todo

		// add
		data_ = data;
	}

	// Retrieve data
	template <typename T>
		requires std::is_fundamental<T>::value
	T Retrieve() const
	{
		DCHECK_F( data_.size() == 2, "wrong number of data members stored in arg %s", data_[0].c_str() );

		T val;
		auto result = std::from_chars( data_[1].data(), data_[1].data() + data_[1].size(), val );
		DCHECK_F( result.ec == std::errc(), "Error coverting data " );
		return val;
	}
	template<>
	bool Retrieve<bool>() const
	{
		int iVal = Retrieve<int>();
		return (bool)iVal;
	}
	const std::vector<std::string>& GetDataStrs() const
	{
		DCHECK_F( !data_.empty(), "Empty data" );
		return data_;
	}
	bool operator==( const RyeParam& rhs ) const
	{
		// Compare by key
		return rhs.data_[0] == data_[0];
	}
	bool operator==( const std::string& rhs ) const
	{
		// Compare by key
		return rhs == data_[0];
	}
private:
	std::vector<std::string> data_;
};


template <typename T>
	requires std::is_fundamental<T>::value
class altRyeParam
{
public:
	altRyeParam( const std::string& key, const T& data )
		: key_(key)
		, data_(data)
	{}

	altRyeParam( std::vector<std::string> data_formatted )
		: key_(data_formatted[0])
	{
		auto result = std::from_chars( data_formatted[1].data(), data_formatted[1].data() + data_formatted[1].size(), data_ );
		DCHECK_F( result.ec == std::errc(), "Error coverting data " );
	}

	// Retrieve data
	T Retrieve() const
	{	
		return data_;
	}
	bool operator==( const altRyeParam& rhs ) const
	{
		// Compare by key
		return rhs.key_ == key_;;
	}
	bool operator==( const std::string& rhs ) const
	{
		// Compare by key
		return rhs == key_;
	}
private:
	std::string key_;
	T data_;
};

// Represents a single parameter file
class RyeFile
{
public:
	RyeFile( std::filesystem::path path, std::string delim = ": ")
		: path_( path )
		, delim_(delim)
	{
		DCHECK_F( path.has_extension(), "RyeFile loaded: %s without extension.", path.string().c_str() );

		// Create file if doesn't exist
		if ( !std::filesystem::exists( path ) )
		{
			std::ofstream f( path );
			DCHECK_F( f.good(), "Bad file creation %s ", path.string().c_str() );
			f.close();
		}
		else // Load all params
		{
			std::ifstream file( path );
			
			std::vector<char> lineBuffer;
			lineBuffer.resize( 512 ); // todo improve to be dynamic
			while ( file.getline( lineBuffer.data(), lineBuffer.size() ) )
			{ // each line								
				std::string linestr( lineBuffer.data() );

				// Remove comments
				auto commentpos = linestr.find( commentChar );
				if ( commentpos != std::string::npos )
				{
					linestr.erase( commentpos, linestr.length() );
				}

				// Skip blank lines
				if ( linestr.length() < delim_.length() + 2 )
				{
					continue;
				}

				// Remove delimiter, and skip lines without delimiter (invalid)
				auto delimpos = linestr.find( delim_ );
				if ( delimpos != std::string::npos )
				{
					linestr.erase( delimpos, delim_.length() );
				}
				else
				{
					continue;
				}			

				std::vector<std::string> separated;

				// Insert key into separated
				separated.push_back( linestr.substr( 0u, delimpos ) );
				linestr.erase( 0u, delimpos );

				// Insert data strs into separated
				while ( ( delimpos = linestr.find( ' ' ) ) != std::string::npos ) 
				{
					separated.push_back(linestr.substr(0, delimpos));
					linestr.erase( 0, delimpos + 1u );
				}

				// Insert trailing data point (no delim after it)
				if(linestr.length() > 0 )
					separated.push_back( linestr );

				// Handle case where there is no data
				if ( separated.size() < 2 )
				{
					LOG_F( WARNING, "Not enough data read %s", separated[0].c_str() );
					continue;
				}

				params_.emplace_back( std::move( separated ) );
			}
		}
	}

	template<typename T>
	bool Resolve( const std::string& key, T& val ) const
	{
		for ( const auto& p : params_ )
		{
			if ( p == key )
			{
				val = p.Retrieve<T>();
				return true;
			}
		}
		return false;
	}
	// Will return either supplied default value (def) or read value if it exists
	template <typename T>
	T ForceResolve( const std::string& key, T def ) const
	{
		Resolve<T>( key, def );
		return def;
	}

	template<typename T>
	bool AppendParam( const std::string& key, const T& data )
	{
		// Return if param already exists
		for ( const auto& p : params_ )
		{
			if ( p == key )
				return false;
		}

		params_.emplace_back( key, data );
		WriteParam( params_.back() );
		return true;
	}
private:
	void WriteParam( const RyeParam& param ) const
	{
		DCHECK_F( std::filesystem::exists( path_ ), "File doesn't exist %s ", path_.string().c_str() );

		std::ofstream file( path_, std::ios::app );
		DCHECK_F( file.good(), "file load fail");

		const auto& data = param.GetDataStrs();
		file << data[0] << delim_;

		for ( auto it = ++data.begin(); it != data.end(); it++ )
		{
			file << *it << ' ';
		}
		file << '\n';

		file.close();
	}
private:
	const std::filesystem::path path_; // includes extension
	const std::string delim_;
	std::vector<RyeParam> params_;
	// character that denotes comment
	static constexpr const char commentChar = '#'; 
};
