
#include "fuzzon_executiontracker.h"
#include "utils/logger.h"

namespace fuzzon {

ExecutionTracker::ExecutionTracker(WorkingMode mode) :
		mode_(mode), cov_(NULL)
{
	Logger::Get("9");
	Logger::Get()->trace("ExecutionTracker::ExecutionTracker");
	if (mode_ == WorkingMode::Monitor)
	{
		shared_memory_ = boost::interprocess::shared_memory_object(
				boost::interprocess::open_or_create,
				shared_memory_buffer_name.c_str(),
				boost::interprocess::read_write);

		shared_memory_.truncate(sizeof(Coverage));
		region_ = boost::interprocess::mapped_region(shared_memory_, boost::interprocess::read_write);
		std::memset(region_.get_address(), 0, region_.get_size());

		cov_ = new(region_.get_address()) Coverage(Coverage::Flow);
	}
	else if (mode_ == WorkingMode::SUT)
	{
		Logger::Get()->trace("WorkingMode::SUT");
		shared_memory_ = boost::interprocess::shared_memory_object(
				boost::interprocess::open_only,
				shared_memory_buffer_name.c_str(),
				boost::interprocess::read_write);
		region_ = boost::interprocess::mapped_region(shared_memory_, boost::interprocess::read_write);

		cov_ = reinterpret_cast<Coverage*>(region_.get_address());
//		cov_ = new(region_.get_address()) Coverage(Coverage::Flow);
	}
	else
	{
		// ..
	}

	return;
}

void ExecutionTracker::Reset()
{
	cov_ = new(region_.get_address()) Coverage(Coverage::Flow);
}


ExecutionTracker::~ExecutionTracker() {
	// TODO Auto-generated destructor stub
}

} /* namespace fuzzon */
