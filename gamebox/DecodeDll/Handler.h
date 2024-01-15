#pragma once


class Handler {

public:

	Handler(){
		successor=0;
	}

	virtual void  * handleRequest(void * pData){

		if (IsAdmin(pData))
		{
		return	handle(pData);
		}

		if (getSuccessor()==0)
		{
			return 0;
		}
		else
			return getSuccessor()->handleRequest(pData);

	}

	virtual void * handle(void * pData)=0;

	virtual bool IsAdmin(void * pData)=0;


	inline  Handler * getSuccessor() {
		return successor;
	}

	inline void setSuccessor(Handler * successor) {
		this->successor = successor;
	}
private:
	Handler * successor;
};

