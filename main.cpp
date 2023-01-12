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
	PerFrameFunctionCaller() {}
	virtual ~PerFrameFunctionCaller() {}

	virtual void CallFunctionsForObjects() = 0;
};


template <typename ObjectType, typename FuncReturnType, typename... FuncArgs>
class SpecifiedPerFrameFunctionCaller : public PerFrameFunctionCaller {

// ctor & dtor
public:
	SpecifiedPerFrameFunctionCaller(){}

// interface
public:
	void SetTimeBudgetPerFrame(double Milliseconds);
	void SetFunctionToCallFromObjects(std::function<FuncReturnType(ObjectType*, FuncArgs... Args)> NewFunction);
	void AddObject(ObjectType*const ObjectToAdd);
	virtual void CallFunctionsForObjects() override;

// members
private:
	std::function<FuncReturnType(ObjectType*, FuncArgs... Args)> AFunctionToCall;

	//Change vector to TSet
	std::vector<ObjectType*> ObjectsToCallAFunctionFor;
	double TimeBudgetInMilliseconds{};
};

template <typename ObjectType, typename FuncReturnType, typename... FuncArgs>
void SpecifiedPerFrameFunctionCaller<ObjectType, FuncReturnType, FuncArgs...>::CallFunctionsForObjects()
{
	for (auto CurrentObject : ObjectsToCallAFunctionFor) {
		AFunctionToCall(CurrentObject, FuncArgs...);
	}

	// For TSet in UE4
	//for (int i = 0; i < ObjectsToCallAFunctionFor.Num(); ++i) {
	//	AFunctionToCall(ObjectsToCallAFunctionFor[i], FuncArgs...);
	//	ObjectsToCallAFunctionFor.remove(i);
	//}
}

template <typename ObjectType, typename FuncReturnType, typename... FuncArgs>
void SpecifiedPerFrameFunctionCaller<ObjectType, FuncReturnType, FuncArgs...>::SetTimeBudgetPerFrame(double Milliseconds)
{
	TimeBudgetInMilliseconds = Milliseconds;
}

template <typename ObjectType, typename FuncReturnType, typename... FuncArgs>
void SpecifiedPerFrameFunctionCaller<ObjectType, FuncReturnType, FuncArgs...>::AddObject(ObjectType*const ObjectToAdd) {
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


	std::vector<PerFrameFunctionCaller*> Callers;

	SpecifiedPerFrameFunctionCaller<FirstClass, void>* FirstClassFunctionCaller = new SpecifiedPerFrameFunctionCaller<FirstClass, void>();
	FirstClassFunctionCaller->SetTimeBudgetPerFrame(4.f);
	FirstClassFunctionCaller->SetFunctionToCallFromObjects(&FirstClass::Hello);
	FirstClassFunctionCaller->AddObject(a);
	FirstClassFunctionCaller->AddObject(b);



	Callers.push_back(FirstClassFunctionCaller);

	SpecifiedPerFrameFunctionCaller<SecondClass, void>* SecondClassFunctionCaller = new SpecifiedPerFrameFunctionCaller<SecondClass, void>();
	SecondClassFunctionCaller->SetTimeBudgetPerFrame(4.f);
	SecondClassFunctionCaller->SetFunctionToCallFromObjects(&SecondClass::Hello);
	SecondClassFunctionCaller->AddObject(c);
	SecondClassFunctionCaller->AddObject(d);
	Callers.push_back(SecondClassFunctionCaller);

	for (PerFrameFunctionCaller* Caller : Callers) {
		Caller->CallFunctionsForObjects();
	}

	return 0;
}