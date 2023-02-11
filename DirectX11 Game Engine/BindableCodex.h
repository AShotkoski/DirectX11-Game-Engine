#pragma once
#include "Bindable.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace Binds
{
	// Singleton
	class Codex
	{
	public:
		template <class tBind, typename... Args>
		static std::shared_ptr<tBind> Resolve( Graphics& gfx, Args&&... args )
		{
			return get().Resolve_<tBind>( gfx, std::forward<Args>(args)... );
		}
	private:
		template <class tBind, typename... Args>
		std::shared_ptr<tBind> Resolve_( Graphics& gfx, Args&&... args )
		{
			const auto key = tBind::GenerateUID( std::forward<Args>( args )... );
			return std::make_shared<tBind>( gfx, std::forward<Args>( args )... );
		}

		static Codex& get()
		{
			static Codex codex;
			return codex;
		}
	private:
		std::unordered_map<std::string, std::shared_ptr<Bindable>> map;
	};

};