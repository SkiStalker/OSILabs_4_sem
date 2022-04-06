#include <iostream>
#include <thread>

using namespace std;

class SyncDLatch
{
private:
	bool sync = 0;
	void tactGen()
	{
		sync++;
		this_thread::sleep_for(chrono::seconds(1));
	}
public:
	SyncDLatch()
	{
		thread(&SyncDLatch::tactGen, this).detach();
	}
	bool Q = false;
	void setD(bool D)
	{
		inline bool S = D;
		inline bool R = !D;
		Q = !(!(sync && S || Q) || sync && R);
	}
};

int main()
{
	return 0;
}