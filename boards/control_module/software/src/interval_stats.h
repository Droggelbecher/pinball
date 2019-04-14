#ifndef interval_stats_h_INCLUDED
#define interval_stats_h_INCLUDED

#include <iostream>

namespace pinball {

template<typename Logger>
class IntervalStats {
	public:
		IntervalStats(const char *name, double interval, Logger& logger) : name_(name), interval_(interval), logger_(logger) {
		}

		void next_frame(double dt) {
			time_passed += dt;
			if(time_passed >= interval_) {
				report();
				time_passed = 0;
			}
		}

		void report() {
			logger_ << name_
				<< " min:" << value_min
				<< " avg:" << value_avg
				<< " max:" << value_max
				<< " count:" << value_count
				<< '\n';
			value_count = 0;
		}

		void add_value(double value) {
			if(!value_count) {
				value_min = 
				value_max = 
				value_avg = value;
				value_count = 1;
				return;
			}

			if(value < value_min) { value_min = value; }
			if(value > value_max) { value_max = value; }

			value_avg =
				value_avg * (value_count / (value_count + 1.0))
				+ value / (value_count + 1.0);

			++value_count;
		}

	private:
		const char *name_;
		double interval_;
		double time_passed = 0;
		int value_count = 0;
		double value_min = 0;
		double value_max = 0;
		double value_avg = 0;
		Logger& logger_;
};

} // ns pinball

#endif // interval_stats_h_INCLUDED

