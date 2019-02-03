#include "DataBaseManager.h"

DataBaseManager *Singleton<DataBaseManager>::sInstance = 0;

#include <iostream>
#include <fstream>

using namespace std;

DataBaseManager::DataBaseManager()
{
}

DataBaseManager::~DataBaseManager()
{
	tinyDoc.Clear();
}

void DataBaseManager::loadDataFromFile(char *fileName)
{
	ifstream myFile;

	std::string name;
	std::string value;

	myFile.open(fileName);

	if(!myFile.fail())
	{
		while(!myFile.eof())
		{
			myFile >> name;

			if(name != "")
			{			
				myFile >> value;
			}

			dataList[name] = value;
		}
	}

	myFile.close();
}

void DataBaseManager::loadDataFromFileList(char *fileList)
{
	ifstream myFile;

	std::string fileName;

	myFile.open(fileList);

	if(!myFile.fail())
	{
		while(!myFile.eof())
		{
			myFile >> fileName;

			if(fileName != "")
			{			
				loadDataFromFile((char*)fileName.c_str());
			}
		}
	}

	myFile.close();
}

bool DataBaseManager::isDataThere(std::string name)
{
	return (dataList.find(name) != dataList.end());
}

std::string DataBaseManager::getDataAsString(std::string name)
{
	if(isDataThere(name))
		return dataList[name];

	return 0;
}

int DataBaseManager::getDataAsInt(std::string name)
{
	int value = 0;

	if(isDataThere(name))
		value = atoi(dataList[name].c_str());

	return value;
}

unsigned int DataBaseManager::getDataAsUInt(std::string name)
{
	unsigned int value = 0;

	if(isDataThere(name))
		value = atoi(dataList[name].c_str());

	return value;
}

void DataBaseManager::addData(std::string name, std::string value)
{
	dataList[name] = value;
}

void DataBaseManager::clearData(void)
{
	dataList.clear();
}

void DataBaseManager::showMe(void)
{
	std::map<std::string, std::string>::iterator itr;

	for(itr = dataList.begin(); itr != dataList.end(); itr++)
	{
		cout << itr->first << " = " << itr->second << endl;
	}

}

void DataBaseManager::loadTable(char *fileName)
{
	TiXmlDocument doc(fileName);

	TiXmlHandle hDoc(&doc);
	TiXmlElement* newTable;

	if(doc.LoadFile())
	{
		newTable = hDoc.FirstChildElement().Element(); // we get the first table

		for( ; newTable; newTable = newTable->NextSiblingElement())
		{
			TiXmlElement *table = newTable->Clone()->ToElement();
			tinyDoc.LinkEndChild(table);
		}
	}
	else
		std::cout << "The XML files (" << fileName << ") didn't open." << std::endl;
}

bool DataBaseManager::doesTableExist(char *name)
{
	bool exist = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement* table;

	table = hTinyDoc.FirstChildElement(name).Element(); // we get the first table

	if(table)
		exist = true;

	return exist;
}

void DataBaseManager::saveTableFull(char *name, char *fileName)
{
	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlDocument doc;
	TiXmlElement* table;

	table = hTinyDoc.FirstChildElement(name).Element(); // we get the first table

	for( ; table; table = table->NextSiblingElement(name)) // then we go through all the tables with the same name
	{
		doc.LinkEndChild(table);
	}

	doc.SaveFile(fileName);
}

void DataBaseManager::addTable(char *name)
{
	TiXmlElement *table = new TiXmlElement(name);

	tinyDoc.LinkEndChild(table);
}

void DataBaseManager::addColumn(char *tableName, char *columnName, bool unique)
{
	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement* table;
	TiXmlElement* newColumn;
	TiXmlElement* siblingColumn;

	// frist we check if the table and column doesn't already exit

	if(doesColumnExist(tableName, columnName)) // if the column already exist
	{
		// throw something
		return;
	}

	// we check if we are the only column in the table
	// then take a sibling column, and we add as many records/rows as it has

	table = hTinyDoc.FirstChildElement(tableName).Element();
	siblingColumn = table->FirstChildElement();
	newColumn = new TiXmlElement(columnName);

	if(siblingColumn) // we do have other cloumns
	{
		TiXmlElement *sibRecord;
		TiXmlElement *newRecord;

		sibRecord = siblingColumn->FirstChildElement();

		for( ; sibRecord; sibRecord = sibRecord->NextSiblingElement())
		{
			// we take the name/id of the sibRecord and use it for the newRecord
			char *sibRecordName = (char*)sibRecord->Value();

			newRecord = new TiXmlElement(sibRecordName);

			newColumn->LinkEndChild(newRecord);
		}
	}
	else // no one is here
	{
		// no need to add any record
	}

	table->LinkEndChild(newColumn);
}

bool DataBaseManager::doesColumnExist(char *tableName, char *columnName)
{
	bool exist = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement* column;

	column = hTinyDoc.FirstChildElement(tableName).FirstChildElement(columnName).Element(); // we try to find the column

	if(column)
		exist = true;

	return exist;
}

bool DataBaseManager::isColumnUnique(char *tableName, char *columnName)
{
	return false;
}

unsigned int DataBaseManager::getColumnSize(char *tableName, char *columnName)
{
	// we will have mulitpal tables, so we will have to check all of them

	unsigned int count = 0;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement* table;
	TiXmlElement* column;
	TiXmlElement* record;

	table = hTinyDoc.FirstChildElement(tableName).Element();

	for(; table; table = table->NextSiblingElement(tableName))
	{
		column = table->FirstChildElement(columnName);

		for(; column; column = column->NextSiblingElement(columnName))
		{
			record = column->FirstChildElement("record");

			for(; record; record = record->NextSiblingElement("record"))
			{
				count++; // we got a record
			}
		}
	}

	return count;
}

unsigned int DataBaseManager::addNewRow(char *tableName)
{
	// this will have to wait till we have a way to keep track of record count
	return 0;
}

bool DataBaseManager::getAllRowNumbersFromTable(char *name, std::vector<std::string> &rowList)
{
	bool weHaveRecords = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement *table;
	TiXmlElement *column;
	TiXmlElement *record;

	table = hTinyDoc.FirstChildElement(name).Element();

	for(; table; table = table->NextSiblingElement(name))
	{
		column = table->FirstChildElement();

		record = column->FirstChildElement();

		for(; record; record = record->NextSiblingElement())
		{
			char* recName = (char*)record->Value();

			rowList.push_back(recName);

			weHaveRecords = true;
		}
	}

	return weHaveRecords;
}

void DataBaseManager::setRecordAsChar(char *tableName, char *columnName, char *rowNumber, char value)
{

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	record->SetAttribute("value", value);

}

void DataBaseManager::setRecordAsUChar(char *tableName, char *columnName, char *rowNumber,  unsigned char value)
{

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	record->SetAttribute("value", value);

}

void DataBaseManager::setRecordAsInt(char *tableName, char *columnName, char *rowNumber,  int value)
{

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	record->SetAttribute("value", value);

}

void DataBaseManager::setRecordAsUInt(char *tableName, char *columnName, char *rowNumber,  unsigned int value)
{

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	record->SetAttribute("value", value);

}

void DataBaseManager::setRecordFloat(char *tableName, char *columnName, char *rowNumber,  float value)
{

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	record->SetDoubleAttribute("value", value);

}

void DataBaseManager::setRecordAsBool(char *tableName, char *columnName, char *rowNumber,  bool value)
{

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	if(value)
		record->SetAttribute("value", "true");
	else
		record->SetAttribute("value", "false");

}

void DataBaseManager::setRecordAsString(char *tableName, char *columnName, char *rowNumber,  std::string value)
{

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	record->SetAttribute("value", (char*)value.c_str());

}

char DataBaseManager::getRecordAsChar(char *tableName, char *columnName, char *rowNumber)
{
	char value;

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	char *stringValue = (char*)record->Attribute("value");
	value = stringValue[0];

	return value;
}

unsigned char DataBaseManager::getRecordAsUChar(char *tableName, char *columnName, char *rowNumber)
{
	unsigned char value;

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	char *stringValue = (char*)record->Attribute("value");
	value = stringValue[0];

	return value;
}

int DataBaseManager::getRecordAsInt(char *tableName, char *columnName, char *rowNumber)
{
	int value;

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	record->QueryIntAttribute("value", &value);

	return value;
}

unsigned int DataBaseManager::getRecordAsUInt(char *tableName, char *columnName, char *rowNumber)
{
	int value;

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	record->QueryIntAttribute("value", &value);

	return value;
}

float DataBaseManager::getRecordAsFloat(char *tableName, char *columnName, char *rowNumber)
{
	float value;

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	record->QueryFloatAttribute("value", &value);

	return value;
}

bool DataBaseManager::getRecordAsBool(char *tableName, char *columnName, char *rowNumber)
{
	bool value = false;
	char *valueString;

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	valueString = (char*)record->Attribute("value");

	std::string str(valueString);

	if(str == "true")
		value = true;

	return value;
}

std::string DataBaseManager::getRecordAsString(char *tableName, char *columnName, char *rowNumber)
{
	std::string value;
	char *valueString;

	TiXmlElement *record = getRecord(tableName, columnName, rowNumber);

	valueString = (char*)record->Attribute("value");

	value = valueString;

	return value;
}

bool DataBaseManager::searchColumnForChar(char *tableName, char *columnName, char value, std::vector<std::string> &rowList)
{
	bool weHaveRecords = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement *table;
	TiXmlElement *column;
	TiXmlElement *record;

	table = hTinyDoc.FirstChildElement(tableName).Element();

	for(; table; table = table->NextSiblingElement(tableName))
	{
		column = table->FirstChildElement(columnName);

		record = column->FirstChildElement();

		for(; record; record = record->NextSiblingElement())
		{
			char* recName = (char*)record->Value();

			if(value == getRecordAsChar(tableName, columnName, recName))
			{
				rowList.push_back(recName);

				weHaveRecords = true;
			}
		}
	}

	return weHaveRecords;
}

bool DataBaseManager::searchColumnForUChar(char *tableName, char *columnName, unsigned char value, std::vector<std::string> &rowList)
{
	bool weHaveRecords = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement *table;
	TiXmlElement *column;
	TiXmlElement *record;

	table = hTinyDoc.FirstChildElement(tableName).Element();

	for(; table; table = table->NextSiblingElement(tableName))
	{
		column = table->FirstChildElement(columnName);

		record = column->FirstChildElement();

		for(; record; record = record->NextSiblingElement())
		{
			char* recName = (char*)record->Value();

			if(value == getRecordAsUChar(tableName, columnName, recName))
			{
				rowList.push_back(recName);

				weHaveRecords = true;
			}
		}
	}

	return weHaveRecords;
}

bool DataBaseManager::searchColumnForInt(char *tableName, char *columnName, int value, std::vector<std::string> &rowList)
{
	bool weHaveRecords = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement *table;
	TiXmlElement *column;
	TiXmlElement *record;

	table = hTinyDoc.FirstChildElement(tableName).Element();

	for(; table; table = table->NextSiblingElement(tableName))
	{
		column = table->FirstChildElement(columnName);

		record = column->FirstChildElement();

		for(; record; record = record->NextSiblingElement())
		{
			char* recName = (char*)record->Value();

			if(value == getRecordAsInt(tableName, columnName, recName))
			{
				rowList.push_back(recName);

				weHaveRecords = true;
			}
		}
	}

	return weHaveRecords;
}

bool DataBaseManager::searchColumnForUInt(char *tableName, char *columnName, unsigned int value, std::vector<std::string> &rowList)
{
	bool weHaveRecords = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement *table;
	TiXmlElement *column;
	TiXmlElement *record;

	table = hTinyDoc.FirstChildElement(tableName).Element();

	for(; table; table = table->NextSiblingElement(tableName))
	{
		column = table->FirstChildElement(columnName);

		record = column->FirstChildElement();

		for(; record; record = record->NextSiblingElement())
		{
			char* recName = (char*)record->Value();

			if(value == getRecordAsUInt(tableName, columnName, recName))
			{
				rowList.push_back(recName);

				weHaveRecords = true;
			}
		}
	}

	return weHaveRecords;
}

bool DataBaseManager::searchColumnForFloat(char *tableName, char *columnName, float value, std::vector<std::string> &rowList)
{
	bool weHaveRecords = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement *table;
	TiXmlElement *column;
	TiXmlElement *record;

	table = hTinyDoc.FirstChildElement(tableName).Element();

	for(; table; table = table->NextSiblingElement(tableName))
	{
		column = table->FirstChildElement(columnName);

		record = column->FirstChildElement();

		for(; record; record = record->NextSiblingElement())
		{
			char* recName = (char*)record->Value();
			cout << "record name = " << recName << endl;
			cout << "record value = " << getRecordAsFloat(tableName, columnName, recName) << endl;

			if(value == getRecordAsFloat(tableName, columnName, recName))
			{
				rowList.push_back(recName);

				weHaveRecords = true;
			}
		}
	}

	return weHaveRecords;
}

bool DataBaseManager::searchColumnForBool(char *tableName, char *columnName, bool value, std::vector<std::string> &rowList)
{
	bool weHaveRecords = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement *table;
	TiXmlElement *column;
	TiXmlElement *record;

	table = hTinyDoc.FirstChildElement(tableName).Element();

	for(; table; table = table->NextSiblingElement(tableName))
	{
		column = table->FirstChildElement(columnName);

		record = column->FirstChildElement();

		for(; record; record = record->NextSiblingElement())
		{
			char* recName = (char*)record->Value();

			if(value == getRecordAsBool(tableName, columnName, recName))
			{
				rowList.push_back(recName);

				weHaveRecords = true;
			}
		}
	}

	return weHaveRecords;
}

bool DataBaseManager::searchColumnForString(char *tableName, char *columnName, std::string value, std::vector<std::string> &rowList)
{
	bool weHaveRecords = false;

	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement *table;
	TiXmlElement *column;
	TiXmlElement *record;

	table = hTinyDoc.FirstChildElement(tableName).Element();

	for(; table; table = table->NextSiblingElement(tableName))
	{
		column = table->FirstChildElement(columnName);

		record = column->FirstChildElement();

		for(; record; record = record->NextSiblingElement())
		{
			char* recName = (char*)record->Value();

			if(value == getRecordAsString(tableName, columnName, recName))
			{
				rowList.push_back(recName);

				weHaveRecords = true;
			}
		}
	}

	return weHaveRecords;
}

void DataBaseManager::showXML(void)
{
	tinyDoc.Print();
}

TiXmlElement* DataBaseManager::getTable(char *name)
{
	// we get the last table
	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement* table;

	table = hTinyDoc.FirstChildElement(name).Element();

	for( ; table; table = table->NextSiblingElement(name))
	{
		// we only use the for loop to get the last table
	}

	if(!table) // check is have a table
	{
		// throw something
	}

	return table;
}

TiXmlElement* DataBaseManager::getColumn(char *tableName, char *columnName)
{
	// we get the column from the last table
	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement* table;
	TiXmlElement* column;

	table = getTable(tableName);

	column = table->FirstChildElement(columnName);

	if(!column) // check is have a column
	{
		// throw something
	}

	return column;
}

TiXmlElement* DataBaseManager::getRecord(char *tableName, char *columnName, char *rowNumber)
{
	// we get the column from the last table
	TiXmlHandle hTinyDoc(&tinyDoc);
	TiXmlElement* table;
	TiXmlElement* column;
	TiXmlElement* record;

	table = hTinyDoc.FirstChildElement(tableName).Element();

	for(; table; table = table->NextSiblingElement(tableName))
	{
		column = table->FirstChildElement(columnName);

		for(; column; column = column->NextSiblingElement(columnName))
		{
			record = column->FirstChildElement(rowNumber);

			if(record)
			{
				return record;
			}
		}
	}

	// if we reach this line then there is no record
	// throw something

	return 0;
}
