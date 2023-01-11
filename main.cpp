#include <iostream>
#include <functional>
#include <vector>

class FirstClass {
public:
	FirstClass(std::string NewToPrint = "") { ToPrint = NewToPrint; }
	void Hello() {
		std::cout << "Info from FirstClass: "<<ToPrint<<"\n";
	}
	std::string ToPrint;
};

class SecondClass {
public:
	SecondClass(std::string NewToPrint = "") { ToPrint = NewToPrint; }
	void Hello() {
		std::cout << "Info from SecondClass: "<<ToPrint<<"\n";
	}
	std::string ToPrint;
};


class PerFrameFunctionCaller {
public:
	virtual void SetTimeBudgetPerFrame(double Milliseconds) = 0;
	virtual void SetFunctionToCallFromObjects() = 0;
	virtual void AddObject() = 0;
	virtual void CallFunctionsForObjects() = 0;

	PerFrameFunctionCaller() {}
	virtual ~PerFrameFunctionCaller() {}
};


template <typename ObjectType, typename FuncReturnType, typename... FuncArgs>
class SpecifiedPerFrameFunctionCaller : PerFrameFunctionCaller {

	std::function<FuncReturnType(ObjectType*, FuncArgs... Args)> AFunctionToCall;

	//Change vector to TSet
	std::vector<ObjectType*> ObjectsToCallAFunctionFor;
	double TimeBudgetInMilliseconds;

public:
	void SetTimeBudgetPerFrame(double Milliseconds);
	void SetFunctionToCallFromObjects(std::function<FuncReturnType(ObjectType*, FuncArgs... Args)> NewFunction);
	void AddObject(ObjectType* ObjectToAdd);
	void CallFunctionsForObjects();
};

template <typename ObjectType, typename FuncReturnType, typename... FuncArgs>
void SpecifiedPerFrameFunctionCaller<ObjectType, FuncReturnType, FuncArgs...>::CallFunctionsForObjects()
{
	for (auto CurrentObject : ObjectsToCallAFunctionFor) {
		AFunctionToCall(CurrentObject, FuncArgs...);
	}
}

template <typename ObjectType, typename FuncReturnType, typename... FuncArgs>
void SpecifiedPerFrameFunctionCaller<ObjectType, FuncReturnType, FuncArgs...>::SetTimeBudgetPerFrame(double Milliseconds)
{
	TimeBudgetInMilliseconds = Milliseconds;
}

template <typename ObjectType, typename FuncReturnType, typename... FuncArgs>
void SpecifiedPerFrameFunctionCaller<ObjectType, FuncReturnType, FuncArgs...>::AddObject(ObjectType* ObjectToAdd) {
	ObjectsToCallAFunctionFor.push_back(ObjectToAdd);
}

template <typename ObjectType, typename FuncReturnType, typename... FuncArgs>
void SpecifiedPerFrameFunctionCaller<ObjectType, FuncReturnType, FuncArgs...>::SetFunctionToCallFromObjects(std::function<FuncReturnType(ObjectType*, FuncArgs... Args)> NewFunction)
{
	AFunctionToCall = NewFunction;
}


int main() {
	FirstClass* a = new FirstClass("Name is a");
	FirstClass* b = new FirstClass("Name is b");
	SecondClass* c = new SecondClass("Name is c");
	SecondClass* d = new SecondClass("Name is d");

	SpecifiedPerFrameFunctionCaller<FirstClass, void> FirstClassFunctionCaller;
	FirstClassFunctionCaller.SetTimeBudgetPerFrame(4.f);
	FirstClassFunctionCaller.SetFunctionToCallFromObjects(&FirstClass::Hello);
	FirstClassFunctionCaller.AddObject(a);
	FirstClassFunctionCaller.AddObject(b);
	FirstClassFunctionCaller.CallFunctionsForObjects();

	SpecifiedPerFrameFunctionCaller<SecondClass, void> SecondClassFunctionCaller;
	SecondClassFunctionCaller.SetTimeBudgetPerFrame(4.f);
	SecondClassFunctionCaller.SetFunctionToCallFromObjects(&SecondClass::Hello);
	SecondClassFunctionCaller.AddObject(c);
	SecondClassFunctionCaller.AddObject(d);
	SecondClassFunctionCaller.CallFunctionsForObjects();

	return 0;
}