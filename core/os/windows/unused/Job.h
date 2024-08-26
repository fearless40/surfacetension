#pragma once

#include "Work.h"


namespace Util
{
	namespace Work
	{

		class Jobs;
		
		static const int MaxNumberOfDependents = 4;

		class Job
		{
		protected:
			std::atomic_int mWaiting;
			typedef std::array<Job *, MaxNumberOfDependents> JArray;
			JArray mDependents;
			int mNextDependent;

			void incrementWaiting();
			void decrementWaiting();


		public:
			Job(WorkFunction func, void * inData = nullptr, void * outData = nullptr);
			Job(WorkItem item);
			Job();
			~Job();

			bool isDoneWaiting();

			// Allow the user to edit. Don't need accessors for this code
			WorkItem mWork;

			// Both functions below are for ease of use
			void setWorkFunction(WorkFunction func);
			void setData(void * inData, void * outData);

			// Sets
			// Adds the item to the list of dependents this class has
			// Incraments the waiting member of the other job
			void addDependent(Job * job);
			void addDependents(Job ** jobs, int nbrJobs);


			friend class Jobs;
		};
	};
};