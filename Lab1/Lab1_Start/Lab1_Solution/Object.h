#include <iostream>
#include <fstream>
#include <string>

namespace W1
{
	class Object 
	{
		int* _intBuffer;

		struct State
		{
			size_t size;
			size_t index;
			std::ostream* _streamP;
			
		} _mystate;

	public:
		Object(int maxSize, std::ofstream* stream)
		{
			this->_mystate.size = maxSize;
			this->_intBuffer = new int[this->_mystate.size];
			this->_mystate._streamP = stream;
			this->_mystate.index = 0;

			if (!this->_intBuffer)
			{
				this->_mystate.size=0;
				this->_intBuffer = nullptr;
			}
		}

		~Object()
		{
			delete[] this->_intBuffer;
		}

		Object& operator+=(int addTo)
		{
			if (this->_mystate.index <= this->_mystate.size)
			{
				this->_intBuffer[this->_mystate.index] = addTo;
				this->_mystate.index++;
			}
			return *this;
		}

		void display() const
		{
			for (size_t i = 0; i < this->_mystate.index; i++)
				*this->_mystate._streamP << i << ":" << this->_intBuffer[i] << std::endl;
		}
	};
}