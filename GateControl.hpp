#pragma once


#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

typedef size_t CardNumber;

typedef size_t GateNumber;



struct Authorization
{
	Authorization() { }
	
	Authorization(CardNumber number, const string& name, const string& startTime, const string& endTime)
	: number_(number), name_(name), startTime_(startTime), endTime_(endTime) { }

	CardNumber number_;

	string name_;
	
	string startTime_;
	
	string endTime_;
};

typedef map<CardNumber, Authorization> AuthorizationMap;
typedef	AuthorizationMap::iterator AuthorizationMapIterator;

typedef	vector<Authorization> AuthorizationVector;

struct Transaction
{
	Transaction() { }

	Transaction(CardNumber number, const string& name, const string& date, const string& time, bool accessAllowed)
	: number_(number), name_(name), date_(date), time_(time), accessAllowed_(accessAllowed) { }

	CardNumber number_;
	
	string name_;
	
	string date_;

	string time_;
	
	bool accessAllowed_;
};

typedef	vector<Transaction> TransactionVector;
typedef	TransactionVector::iterator TransactionVectorIterator;

class	GateControl
{
	public:
		bool accessAllowed(CardNumber number);
	
		bool addAuthorization(CardNumber number, const string& name, const string& startTime, const string& endTime);
		
		bool changeAuthorization(CardNumber number, const string& name, const string& startTime, const string& endTime);
		
		bool deleteAuthorization(CardNumber number);
		
		void getAllAuthorizations(AuthorizationVector& authorizationVector);
		
		void getAllTransactions(TransactionVector& transactionVector);
	
		bool getCardAuthorization(CardNumber number, Authorization& authorization);

		void getCardTransactions(CardNumber number, TransactionVector& transactionVector);

	private:
		AuthorizationMap authorizationMap_;
		
		TransactionVector transactionVector_;
};



extern string gCurrentDate;

extern string gCurrentTime;



//****************************************************************************************
//
//	TO DO
//
//****************************************************************************************


bool GateControl::accessAllowed(CardNumber number)
{
	
	//	returns true if access is permitted
	// returns false if access is denied
	// recorded in transactionlog
	
	{
		auto it = authorizationMap_.find(number);
		bool accessAllowedt;
		if (it == authorizationMap_.end())
		{
			accessAllowedt = false;
			Transaction tempTrans(number, "***", gCurrentDate, gCurrentTime, accessAllowedt);
			transactionVector_.push_back(tempTrans);
			return accessAllowedt;
		}
		Authorization tempAuth = it->second;
		int start = stoi(tempAuth.startTime_);
		int end = stoi(tempAuth.endTime_);
		int current = stoi(gCurrentTime);
		if ((it != authorizationMap_.end()) && (start <= current) && (end >= current))
		{
			accessAllowedt = true;
		}
		else
		{
			accessAllowedt = false;
		}
		Transaction tempTrans(tempAuth.number_, tempAuth.name_, gCurrentDate, gCurrentTime, accessAllowedt);
		transactionVector_.push_back(tempTrans);
		return accessAllowedt;
	}
}


bool GateControl::addAuthorization(CardNumber number, const string& name, const string& startTime, const string& endTime)
{
	
	//	return false if item is already present
	// return true if not and add itme to the map


	auto ret = authorizationMap_.insert(pair<CardNumber, Authorization>(number, Authorization(number, name, startTime, endTime)));
	return ret.second;

}


bool GateControl::changeAuthorization(CardNumber number, const string& name, const string& startTime, const string& endTime)
{
	
	// false if the card could not be found
	// true if found, and changes the name and time range of the existing card
	auto it = authorizationMap_.find(number);
	AuthorizationMap tempmap;
	for (auto ret = authorizationMap_.begin(); ret != authorizationMap_.end(); ++ret)
	{
		if (it == ret)
		{
			tempmap.insert(pair<CardNumber, Authorization>(number, Authorization(number, name, startTime, endTime)));
		}
		else
		{
			tempmap.insert(pair<CardNumber, Authorization>(ret->first, ret->second));
		}
	}
	if (it != authorizationMap_.end())
	{
		
		authorizationMap_.clear();
		for (auto k = tempmap.begin(); k != tempmap.end(); ++k)
		{
			authorizationMap_.insert(pair<CardNumber, Authorization>(k->first, k->second));
		}
		return true;
	}
	else
	{
		return false;
	}

}


bool GateControl::deleteAuthorization(CardNumber number)
{
	//	returns false if the card was not found and couldnt be deleted
	// returns true when found, and is  deleted
	if (authorizationMap_.erase(number) > 0)
	{
		return true;
	}
	else
	{
		return false;
	}

}


void GateControl::getAllAuthorizations(AuthorizationVector& authorizationVector)
{

	//	vector should be cleared if there are no authorization records
	// Doesnt return anything, if there are records, the vector should be filled by all authorization records
	if (authorizationMap_.empty())
	{
		authorizationVector.clear();
	}

	else
	{
	//	it AuthorizationMapIterator;
		authorizationVector.clear();
	for (auto it = authorizationMap_.begin(); it != authorizationMap_.end(); ++it)
	{
		authorizationVector.push_back(it->second);
	}
	}
}


void GateControl::getAllTransactions(TransactionVector& transactionVector)
{
	
	//	vector should be cleared if there are no tranaction records
	// Doesnt return anything, if there are records, the vector should be filled by all transaction records
	if (transactionVector_.empty())
	{
		transactionVector.clear();
	}
	else
	{
		transactionVector.clear();
		TransactionVectorIterator it;
		for (it = transactionVector_.begin(); it != transactionVector_.end(); ++it)
		{
			Transaction tempTrans(it->number_, it->name_, it->date_, it->time_, it->accessAllowed_);
			transactionVector.push_back(tempTrans);
		}
	}

}


bool GateControl::getCardAuthorization(CardNumber number, Authorization& authorization)
{

	//	return false if the card was not found
	//  return true if found
	// And authorization record is filed 
	auto it = authorizationMap_.find(number);
	if (it != authorizationMap_.end())
	{
		authorization.name_ = (it->second).name_;
		authorization.startTime_ = (it->second).startTime_;
		authorization.endTime_ = (it->second).endTime_;
		return true;
	}
	else
	{
		return false;
	}

}


void GateControl::getCardTransactions(CardNumber number, TransactionVector& transactionVector)
{
	
	//	vector should be cleared if there are no tranaction records
	// Doesnt return anything, if there are records, the vector should be filled by all transaction records
	if (transactionVector_.empty())
	{
		transactionVector.clear();
	}
	else
	{
		TransactionVectorIterator it;
		for (it = transactionVector_.begin(); it != transactionVector_.end(); ++it)
		{
			if (it->number_ == number)
			{
				Transaction tempTrans(it->number_, it->name_, it->date_, it->time_, it->accessAllowed_);
				transactionVector.push_back(tempTrans);
			}
		}
	}
}

