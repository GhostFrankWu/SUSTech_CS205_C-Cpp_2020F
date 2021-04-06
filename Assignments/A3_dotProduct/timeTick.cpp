
#include<chrono>

class TimePoint{ //时间结构体
private:
	std::chrono::time_point<std::chrono::system_clock> value;
public:
	TimePoint() {
		value = std::chrono::system_clock::now();
	}
	void setTime() {
		value = std::chrono::system_clock::now();
	}
	constexpr long long timeDffer(std::chrono::time_point<std::chrono::system_clock> T) {
		return std::chrono::duration_cast<std::chrono::microseconds>(T-value).count();
	}
	constexpr long long timeDffer() {
		return std::chrono::duration_cast<std::chrono::microseconds>
			(std::chrono::system_clock::now()-value ).count();
	}
};