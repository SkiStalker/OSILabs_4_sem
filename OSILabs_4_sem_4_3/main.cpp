#include <iostream>
#include <thread>

using namespace std;

class SyncDLatch
{
private:
	bool Q = false;
	bool sync = 0;
	thread thr;
	void tactGen()
	{
		while (true)
		{
			cout << Q << endl;
			sync++;
			this_thread::sleep_for(chrono::seconds(1));
		}
	}
public:
	SyncDLatch()
	{
		thr = thread(&SyncDLatch::tactGen, this);
		thr.detach();
	}
	void setD(bool D)
	{
		bool S = D;
		bool R = !D;
		Q = !(!(sync && S || Q) || sync && R);
	}
};

int main()
{
	SyncDLatch sd;

	while (true)
	{
		string buf;

		getline(cin, buf);

		if (buf == "0")
			sd.setD(0);
		else if (buf == "1")
			sd.setD(1);

	}
	return 0;
}