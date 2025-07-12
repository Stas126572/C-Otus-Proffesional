#include <stdio.h>
#include <iostream>
#include <map>

void assert(bool conditional, const char* message)
{
	if (!conditional)
	{
		std::cout << "Assert error: " << message << std::endl;
		throw message;
	};
}

void LOG(const char* message)
{
	std::cout << message << std::endl;
}

template<typename Type, int automatic_val, int demension, int start_demension = demension>
class Matrix;


//TODO : WRITE AN ASSERT: If start_demension < demension.
template<typename Type, int demension, int start_demension = demension>
class MatrixBasic
{
	friend class Matrix<Type, demension, -1, start_demension>;
	using StartMatrix                 =  MatrixBasic<Type, start_demension, start_demension>;
        using MatrixWithEqualDemension    =  MatrixBasic<Type, demension      , start_demension>;
        using MatrixWithSmallDemension    =  MatrixBasic<Type, demension - 1  , start_demension>;
        using MatrixWithEqualDemensionRef =  MatrixWithEqualDemension*;
        using MatrixWithSmallDemensionRef =  MatrixWithSmallDemension*;

	StartMatrix*                                     basic_matrix;
	Type                                             val;
	std::map<int, MatrixWithSmallDemensionRef>       mp;
protected:
        using StartMatrixRef                              =  StartMatrix*;
	bool IsItSet = false;

	bool is_it_have(int index)
	{
		return (mp.count(index) != 0);
	}

	Type get_value()
        {
                static_assert(demension <= 0,                  "Please don't take val at element, with index bigger 0.");
                return val;
        }
        MatrixWithSmallDemensionRef get_element (int index)
        {
                static_assert(demension > 0,                 "Please don't take element for index at matrix with zero dementional. It is an element.");
                assert       (mp.count(index) != 0,          "There is not element with this index");
                return mp[index];
        };
        MatrixWithEqualDemensionRef set_value(Type tp)
        {
                LOG("Val: ");
		IsItSet = true;
                val = tp;
                return this;
        }

        MatrixWithSmallDemensionRef set_element(int index) 
        {
                LOG("Index: ");
                mp[index] = new MatrixWithSmallDemension(basic_matrix); 
                return mp[index];
        }
	StartMatrixRef get_start_matrix()
	{
		return basic_matrix;
	}
public:
	MatrixBasic ()
	{
		static_assert(start_demension == demension, "Please get ref to parent at not max-demensionmatrix.");
		basic_matrix = this;
	}
	MatrixBasic (StartMatrixRef mb)
	{
		static_assert(start_demension != demension, "Please don't get ref to max-demensional matrix.");
		basic_matrix = mb;
	};
	bool operator<(MatrixWithEqualDemension mt)
	{
		return false;
	}
};

template<typename Type, int automatic_val, int demension, int start_demension>
class Matrix : public MatrixBasic<Type, demension, start_demension>
{
	std::array<int, start_demension - demension> ar;
	template<typename T, int times = 0>
	void set(Type val, T ret = MatrixBasic<Type, demension, start_demension>::get_start_matrix())
	{

		if constexpr(times == start_demension)
		{
			ret->set_value(val);
			return;
		}
		else
		{
			if (ret->is_it_have(ar[times]))
			{
				set(val, ret->get_element(ar[times]));
			}
			else
			{
				set(val, ret->set_element(ar[times]));
			}
			return;
		}
	}

	template<typename T, int times = 0>
	Type get(T ret = MatrixBasic<Type, demension, start_demension>::get_start_matrix())
	{

		if constexpr(times == start_demension)
		{
			if (MatrixBasic<Type, demension, start_demension>::IsItSet)
			{
				return ret->get_val();
			}
			else
			{
				return automatic_val;
			}
		}
		else
		{
			if (ret->get_element(ar[times]))
			{
				return get<MatrixBasic<Type,start_demension - times - 1 ,start_demension>, times + 1>(ret->get_element(ar[times]));
			}
			else
			{
				return automatic_val;
			};
		}
	}
public:
	Matrix()
	{
		MatrixBasic<Type, demension, start_demension>();
	}
	Matrix(MatrixBasic<Type, start_demension, start_demension>* mb, std::array<int, start_demension - demension - 1> ar_, int index)
	{
		for (int i = 0; i < ar_.size(); i++)
		{
			ar[i] = ar_[i];
		}
		ar[start_demension - demension] = index;
		MatrixBasic<Type, demension, start_demension>(mb);
	}
	Matrix<Type, automatic_val, demension - 1, start_demension>* operator[] (int index)
	{
		Matrix<Type, automatic_val, demension - 1, start_demension> mt = new Matrix<Type, automatic_val, demension - 1, start_demension>(get_start_matrix(), ar, index);
		return mt;
	};
	void operator= (Type val)
	{
		set(val);
	}

	operator Type()
	{
		return get();
	}
};

int main()
{
	try
	{
		Matrix<int, -1, 2>* mb = new Matrix<int, -1, 2>();
		/*
		MatrixBasic<int, 2>* mb = new MatrixBasic<int, 2>();
		mb->set_element_(0)->set_element_(0)->set_element(1);
		std::cout << mb->get_element(0)->get_element(0)->get_value();
		//MatrixBasic<int, 2>* mb_ = new MatrixBasic<int, 2>(mb);
		*/
		return 0;
	}
	catch(...)
	{
		return 1;
	}
}

