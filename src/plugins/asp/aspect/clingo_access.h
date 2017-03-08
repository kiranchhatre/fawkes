/***************************************************************************
 *  clingo_access.h - Clingo access wrapper for the aspects
 *
 *  Created: Mon Oct 31 13:41:07 2016
 *  Copyright  2016 Björn Schäpers
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version. A runtime exception applies to
 *  this software (see LICENSE.GPL_WRE file mentioned below for details).
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL_WRE file in the doc directory.
 */

#ifndef __PLUGINS_ASP_ASPECT_CLINGO_ACCESS_H_
#define __PLUGINS_ASP_ASPECT_CLINGO_ACCESS_H_

#include <atomic>
#include <clingo.hh>
#include <functional>
#include <memory>
#include <vector>

#include <core/threading/mutex.h>

namespace fawkes {
#if 0 /* just to make Emacs auto-indent happy */
}
#endif

class Logger;

class ClingoAccess
{
	private:
	Logger* const Log;
	const std::string LogComponent;
	int NumberOfThreads;
	bool Splitting;

	Mutex ControlMutex;
	Clingo::Control *Control;

	mutable Mutex ModelMutex;
	Clingo::SymbolVector ModelSymbols, OldSymbols;
	unsigned int ModelCounter;

	std::atomic_bool Solving;
	mutable Mutex CallbackMutex;
	std::vector<std::shared_ptr<std::function<bool(void)>>> ModelCallbacks;
	std::vector<std::shared_ptr<std::function<void(Clingo::SolveResult)>>> FinishCallbacks;
	Clingo::GroundCallback GroundCallback;

	bool newModel(const Clingo::Model& model);
	void solvingFinished(const Clingo::SolveResult result);

	void allocControl(void);

	public:
	enum DebugLevel_t
	{
		None = 0,
		Time = 10,
		Programs = 20,
		Externals = 30,
		Models = 40,
		AllModelSymbols = 50,
		All,
		EvenClingo
	};

	std::atomic<DebugLevel_t> DebugLevel;

	ClingoAccess(Logger *log, const std::string& logComponent);
	~ClingoAccess(void);

	void registerModelCallback(std::shared_ptr<std::function<bool(void)>> callback);
	void unregisterModelCallback(std::shared_ptr<std::function<bool(void)>> callback);
	void registerFinishCallback(std::shared_ptr<std::function<void(Clingo::SolveResult)>> callback);
	void unregisterFinishCallback(std::shared_ptr<std::function<void(Clingo::SolveResult)>> callback);

	void setGroundCallback(Clingo::GroundCallback&& callback);

	bool solving(void) const noexcept;
	bool startSolving(void);
	bool startSolvingBlocking(void);
	bool cancelSolving(void);

	bool reset(void);

	void setNumberOfThreads(const int threads, const bool useSplitting = false);
	int numberOfThreads(void) const noexcept;

	Clingo::SymbolVector modelSymbols(void) const;

	bool loadFile(const std::string& path);

	bool ground(const Clingo::PartSpan& parts);

	inline bool assign_external(const Clingo::Symbol& atom, const bool value)
	{
		return assign_external(atom, value ? Clingo::TruthValue::True : Clingo::TruthValue::False);
	}

	inline bool free_exteral(const Clingo::Symbol& atom)
	{
		return assign_external(atom, Clingo::TruthValue::Free);
	}

	bool assign_external(const Clingo::Symbol& atom, const Clingo::TruthValue value);
	bool release_external(const Clingo::Symbol& atom);
};

} // end namespace fawkes

#endif
