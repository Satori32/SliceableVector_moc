#include <iostream>
#include <cstdint>
#include <vector>
#include <memory>

#include <exception>
#include <stdexcept>

template<class T>
class Vector {
public:

	Vector() {}
	Vector(const std::initializer_list<T>& In) { D = std::make_shared<std::vector<T>>(In.begin(), In.end()); L = In.size(); }

	bool Clear() {
		D->clear();
		F = 0;
		L = 0;
	}
	/**/
	bool Resize(std::size_t N) {
		
		if (IsShadow) {
			if (N < L) { L = N; return true; }
			if ((F + N) >= D->size()) {
				L = D->size() - F;
			}
			else {
				L = N;
			}
		}
		else {
			D->resize(F + N);
		}
		return true;
	}
	/**/
	T& Back() {
		return (*D)[F + L];
	}

	T& Front() {
		return (*D).front();
	}

	std::size_t Size() {
		return L;
	}

	bool PushBack(const T& In) {
		if (!IsShadow) {
			D->push_back(In);
			if (D->size()>L+F) { L++; }
		}
		else {
			return false;
		}
		return true;
	}
	bool PopBack() {
		if (!IsShadow) {
			D->pop_back();
			if (D->size()) { L--; }

		}
		else {
			return false;
		}
		return true;
	}

	T& operator [](std::size_t In) {
		return (*D)[F + In];
	}

	typename std::vector<T>::const_iterator begin() {
		return D->cbegin() + F;
	}
	typename std::vector<T>::const_iterator end() {
		if ((F + L) >= D->size()) { return D->cend(); }
		return D->cbegin() + F + L;
	}

	Vector<T> Slice(std::intmax_t Pos, std::size_t Len) {
		if (D->size() <= F + Pos + Len) { throw std::logic_error("error in Slice."); }
		if (Pos + F < 0) { throw std::logic_error("error in Slice."); }
		Vector<T> X = *this;
		X.F += Pos;
		X.L = Len;
		X.IsShadow = true;
		return X;
	}

protected:
	std::shared_ptr<std::vector<T>> D = std::make_shared<std::vector<T>>();
	std::size_t F = 0;
	std::size_t L = 0;
	bool IsShadow = false;//shadow is crashable and not care. your own risk.

};

int main() {
	Vector<int> V{1,2};
	V.PushBack(3);

	auto X = V.Slice(1, 1);
	auto Y = X.Slice(-1, 1);

	return 0;
}