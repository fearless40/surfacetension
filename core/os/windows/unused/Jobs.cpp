#include "../stdafx.h"
#include "Lock.h"
#include "Job.h"
#include "Jobs.h"
#include "ThreadPool.h"

using namespace Util::Work;


void Jobs::JobSubmitDependentWork( WorkData data ) {
    Jobs * jobs = reinterpret_cast<Jobs *>(data.inData);
    Job  * job  = reinterpret_cast<Job  *>(data.outData);
    
    // Actually call the user code
    job->mWork.func( job->mWork.data );
	
    
    // Now signify to the Jobs class that the dependencies need to be completed.
	jobs->JobDependentWorkCompleted(job);
    
    // Now delete the job
    delete job;
}

Jobs::Jobs( ThreadPool & pool ) : mPool( pool ), SubmitJobsThatDependenciesHaveBeenMetCounter(0)
{
    // Initalize the free list
    mFreeList.reserve(MaxNumberOfWaitingJobs);
    for( int loop = MaxNumberOfWaitingJobs - 1 ; loop >= 0; --loop) {
        mFreeList.push_back(loop);
    }

	// Initalize the dependent array
	for (int loop = 0; loop < MaxNumberOfWaitingJobs; ++loop) {
		mDependents[loop] = nullptr;
	}
}

void Jobs::createSubmit(WorkFunction func, void * inData, void * outData)
{
    mPool.submitWork( WorkItem( func, inData, outData) );
}

void Util::Work::Jobs::createSubmit(WorkItem item)
{
	mPool.submitWork(item);
}

void Jobs::submit(Job * job) {
    
    // This means the job is waiting on some other job to complete and may already be in the que
    assert( job->mWaiting == 0 );
    
    // If the job has no dependents then just submit it and remove it
    if( job->mNextDependent == 0 ) {
        mPool.submitWork( job->mWork );
        delete job;  // Ok to delete because the thread pool holds onto the data
		return;
    }
    

    // Job has dependents
    
    Lock<CriticalSection> lock( mCS );
    for( int loop = 0; loop < job->mNextDependent; ++loop )
    {
        // DONT Add the children
        // addDependents(job->mDependents[loop]);

        // Now add the child(dependent item) to the list, use the free list if non 0
		mDependents[mFreeList.back()] = job->mDependents[loop];
        mFreeList.pop_back();
    };
    
    // Submit it with a job item wrapper
    mPool.submitWork( WorkItem( Jobs::JobSubmitDependentWork,  this, job ) );
    
}

void Jobs::JobDependentWorkCompleted( Job * job ) {
    // Go through the list of dependents and decrement work waiting number
    for( int loop = 0; loop < job->mNextDependent; ++loop ) {
        job->mDependents[loop]->decrementWaiting();
    }
    
    // Make sure that we don't have a hundred SubmitJobsThatDependenciesHaveBeenMet (only need one running at a time)
    if( SubmitJobsThatDependenciesHaveBeenMetCounter == 0 ) {
        // Submit the work
        mPool.submitWork( wiHelper(SubmitJobsThatDependenciesHaveBeenMet, this, nullptr ));
    }
    
}

void Jobs::SubmitJobsThatDependenciesHaveBeenMet( WorkData data ) {
    Jobs * jobs = reinterpret_cast<Jobs *> (data.inData);
    
    ++jobs->SubmitJobsThatDependenciesHaveBeenMetCounter;
    
    Lock<CriticalSection> lock( jobs->mCS );
    
    // Always scan the entire array
    for( int loop = 0; loop < Jobs::MaxNumberOfWaitingJobs; ++loop ) {
        if( jobs->mDependents[loop] && jobs->mDependents[loop]->mWaiting == 0 ) {
            // We can submit the job
            Job * jTemp = jobs->mDependents[loop]; 
            jobs->mDependents[loop] = nullptr;
            jobs->mFreeList.push_back(loop);
            jobs->submit( jTemp );
        }
    }
        
    --jobs->SubmitJobsThatDependenciesHaveBeenMetCounter;
}





















