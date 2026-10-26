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
	Matrix(int row, int col, int top, int bottom): row(row), col(col) {
		std::random_device r;
		std::default_random_engine e1(r());
		std::uniform_int_distribution<int> uniform_dist(top, bottom);
		std::vector<T> means;
		for (int i = 0; i < row*col; i++) {
			int mean = uniform_dist(e1);
			means.push_back(mean);
		}
		for (int i = 0; i < row * col; i++) {
			add(arrayMatrix, means[i]);
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
			for (int i = 0; i < m.col; ++i) {
				os << tmp->value << " ";
				tmp = tmp->next;
			}
			os << "\n";
		}
		return os;
	}
	T operator()(int indexRow, int indexCol) const{
		if (indexRow < 0 || indexRow >= this->row ||
			indexCol < 0 || indexCol >= this->col) {
			throw std::runtime_error("Incorrect index 1");
		}

		// Вычисляем позицию элемента в списке
		int position = indexRow * this->col + indexCol;

		Array<T>* tmp = this->arrayMatrix;

		for (int i = 0; i < position; ++i) {
			tmp = tmp->next;
		}
		return tmp->value;
	}
	void operator()(int indexRow, int indexCol, T mean){
		if (indexRow < 0 || indexRow >= this->row ||
			indexCol < 0 || indexCol >= this->col) {
			throw std::runtime_error("Incorrect index 2");
		}

		int position = indexRow * this->col + indexCol;

		Array<T>* tmp = this->arrayMatrix;

		for (int i = 0; i < position; ++i) {
			tmp = tmp->next;
		}
		tmp->value = mean;
	}

	void operator+=(const Matrix<T>& other) {
		if (this->row == other.row && this->col == other.col) {
			Array<T>* tmp = this->arrayMatrix;
			Array<T>* tmpOther = other.arrayMatrix;
			while (tmp->next) {
				tmp->value += tmpOther->value;
				tmp = tmp->next;
				tmpOther = tmpOther->next;
			}
			tmp->value += tmpOther->value;
		}
		else {
			throw std::runtime_error("The matrices have different sizes.");
		}
	}
	friend Matrix<T> operator+(const Matrix<T>& m, const Matrix<T>& o) {
		Matrix<T> tmp = m;
		tmp += o;
		return tmp;
	}

	void operator-=(const Matrix<T>& other) {
		if (this->row == other.row && this->col == other.col) {
			Array<T>* tmp = this->arrayMatrix;
			Array<T>* tmpOther = other.arrayMatrix;
			while (tmp->next) {
				tmp->value -= tmpOther->value;
				tmp = tmp->next;
				tmpOther = tmpOther->next;
			}
			tmp->value -= tmpOther->value;
		}
		else {
			throw std::runtime_error("The matrices have different sizes.");
		}
	}
	friend Matrix<T> operator-(const Matrix<T>& m, const Matrix<T>& o) {
		Matrix<T> tmp = m;
		tmp -= o;
		return tmp;
	}

	void operator*=(T mean) {
		Array<T>* tmp = this->arrayMatrix;
		while (tmp->next) {
			tmp->value *= mean;
			tmp = tmp->next;
		}
		tmp->value *= mean;
	}
	friend Matrix<T> operator*(const Matrix<T>& m, T mean) {
		Matrix<T> ans = m;
		ans *= mean;
		return ans;
	}
	friend Matrix<T> operator*(T mean, const Matrix<T>& m) {
		Matrix<T> ans = m;
		ans *= mean;
		return ans;
	}

	void operator/=(T mean) {
		Array<T>* tmp = this->arrayMatrix;
		while (tmp->next) {
			tmp->value /= mean;
			tmp = tmp->next;
		}
		tmp->value *= mean;
	}
	friend Matrix<T> operator/(const Matrix<T>& m, T mean) {
		Matrix<T> ans = m;
		ans /= mean;
		return ans;
	}

	int tr() {
		if (this->row == this->col) {
			int result{};
			for (int i = 0; i < this->row * this->col; i += (this->col+1)) {
				result += (*this)(i);
			}
			return result;
		}
		else {
			throw std::runtime_error("The matrix must be square");
		}
	}
	friend Matrix<T> operator*(const Matrix<T>& a, const Matrix<T>& other) {
		if (a.col == other.row) {
			Matrix<T> tmp(a.row, other.col, 0, 0);
			for (int k = 0; k < a.row; ++k) {
				for (int i = 0; i < other.col; ++i) {
					T result = 0;
					for (int j = 0; j < a.col; ++j) {
						result += a(k,j) * other(j,i);
					}
					tmp(k, i, result);
				}
			}
			return tmp;
		}
		else {
			throw std::runtime_error("The matrices are not adjacent.");
		}
	}
};


const float Matrix<float>::deff = 0.000001f; //придумать как сделать частичную специализацию для оператора. в принципе как специализировать для типов с плавающей точкой 

int main() {
	try {
		Matrix<int> A(3, 3, 1, 6);
		Matrix<int> C(3, 5, 1, 9);
		std::cout << A << "\n";
		std::cout << C << "\n";
		Matrix<int> D = A * C;
		std::cout << D;
	}
	catch (const std::exception& e) {
		std::cerr << e.what();
	}
}