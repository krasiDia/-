import std;
#include <cstdarg>

//структура для связанного списка
template<typename T>
struct Array {
	T value{};
	Array<T>* next = nullptr;
};

//добавление в связанный список
template<typename T>
void add(Array<T>*& head, T value) {
	Array<T>* tmp = head;
	Array<T>* newEl = new Array<T>(value, nullptr);
	if (!head) {
		head = newEl;
		return;
	}
	while (tmp->next) {
		tmp = tmp->next;
	}
	tmp->next = newEl;
}


template<typename T>
class Matrix {
	Array<T>* arrayMatrix = nullptr;
	int row{}, col{};
	static const float deff;
public:
	Matrix() = default;
	Matrix(int row, int col, ...): row(row), col(col) {
		va_list list;
		va_start(list, col);	
		for (int i = 0; i < row*col; i++) {
			add(arrayMatrix, va_arg(list, T));
		}		
	}
	Matrix(const Matrix& m) {
		if (!m.arrayMatrix) {
			this->arrayMatrix = nullptr;
			this->row = 0;
			this->col = 0;
		}
		else {
			this->row = m.row;
			this->col = m.col;

			Array<T>* tmp = m.arrayMatrix;
			while (tmp->next) {
				add(this->arrayMatrix, tmp->value);
				tmp = tmp->next;
			}
			add(this->arrayMatrix, tmp->value);
		}
	}
	bool operator==(const Matrix& m) {
		Array<T>* tmp = m.arrayMatrix;
		while (tmp->next) {
			if (tmp->value != this->arrayMatrix->value) {
				return false;
			}
			tmp = tmp->next;
			this->arrayMatrix = this->arrayMatrix->next;
		}
		if (tmp->value != this->arrayMatrix->value) {
			return false;
		}
		return true;
	}
	bool operator!=(const Matrix& m) {
		return !(*this == m);
	}
	friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
		Array<T>* tmp = m.arrayMatrix;
		while (tmp) {
			for (int i = 0; i < m.row; ++i) {
				os << tmp->value << " ";
				tmp = tmp->next;
			}
			os << "\n";
		}
		return os;
	}
};

const float Matrix<float>::deff = 0.000001f; //придумать как сделать частичную специализацию для оператора

int main() {
	Matrix<int> A(2, 2, 1, 2, 3, 4);
	Matrix<int> B(A);
	Matrix<int> C(2, 2, 1, 2, 3, 7);
	//std::cout << (A != C);
	std::cout << A;
}