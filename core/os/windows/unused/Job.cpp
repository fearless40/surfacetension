//#include "../stdafx.h"

#include <array>
#include <atomic>
#include <assert.h>
#include "Job.h"

void Util::Work::Job::incrementWaiting()
{
	mWaiting++;
}

void Util::Work::Job::decrementWaiting()
{
	mWaiting--;
}

Util::Work::Job::Job(WorkFunction func, void * inData, void * outData) :
	mNextDependent(0), mWaiting(0)
{
	mWork.func = func;
	mWork.data.inData = inData;
	mWork.data.outData = outData;
}

Util::Work::Job::Job(WorkItem item) : mWork(item), mNextDependent(0), mWaiting(0)
{

}

Util::Work::Job::Job() : mNextDependent(0), mWaiting(0)
{
}

Util::Work::Job::~Job()
{
	// If the job is being deleted before completion an error should pop up
	assert(mWaiting == 0);
}

bool Util::Work::Job::isDoneWaiting()
{
	if( mWaiting == 0 ) return true;
	return false;
}

void Util::Work::Job::setWorkFunction(WorkFunction func)
{
	mWork.func = func;
}

void Util::Work::Job::setData(void * inData, void * outData)
{
	mWork.data.inData = inData;
	mWork.data.outData = outData;
}

void Util::Work::Job::addDependent(Job * job)
{
	// Make sure we don't create more dependents that we have allocated
	assert(mNextDependent < MaxNumberOfDependents);

	job->incrementWaiting();
	
	mDependents[mNextDependent++] = job;

}

void Util::Work::Job::addDependents(Job ** jobs, int nbrJobs)
{
	// Make sure we don't create more dependents that we have allocated
	assert(mNextDependent + nbrJobs < MaxNumberOfDependents);

	for (int counter = 0; counter < nbrJobs; ++counter) {
		jobs[counter]->incrementWaiting();
		mDependents[mNextDependent++] = jobs[counter];
	}
	

}
