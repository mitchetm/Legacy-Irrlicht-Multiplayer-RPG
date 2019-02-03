#pragma once

#include "tinyxml.h"

#include "Singleton.h"

#include <map>
#include <string>
#include <vector>

class DataBaseManager : public Singleton<DataBaseManager>
{

public:

	DataBaseManager();
	~DataBaseManager();

	void loadDataFromFile(char *fileName);
	void loadDataFromFileList(char *fileList);

	bool isDataThere(std::string name);

	std::string getDataAsString(std::string name);
	int getDataAsInt(std::string name);
	unsigned int getDataAsUInt(std::string name);

	void addData(std::string name, std::string value);

	void clearData(void);

	void showMe(void);

	void loadTable(char *fileName);
	bool doesTableExist(char *name);
	void saveTableFull(char *name, char *fileName);
	void addTable(char *name);
	void addColumn(char *tableName, char *columnName, bool unique = false);
	bool doesColumnExist(char *tableName, char *columnName);
	bool isColumnUnique(char *tableName, char *columnName);
	unsigned int getColumnSize(char *tableName, char *columnName);
	unsigned int addNewRow(char *tableName);

	bool getAllRowNumbersFromTable(char *name, std::vector<std::string> &rowList);

	void setRecordAsChar(char *tableName, char *columnName, char *rowNumber, char value);
	void setRecordAsUChar(char *tableName, char *columnName, char *rowNumber,  unsigned char value);
	void setRecordAsInt(char *tableName, char *columnName, char *rowNumber,  int value);
	void setRecordAsUInt(char *tableName, char *columnName, char *rowNumber,  unsigned int value);
	void setRecordFloat(char *tableName, char *columnName, char *rowNumber,  float value);
	void setRecordAsBool(char *tableName, char *columnName, char *rowNumber,  bool value);
	void setRecordAsString(char *tableName, char *columnName, char *rowNumber,  std::string value);

	char getRecordAsChar(char *tableName, char *columnName, char *rowNumber);
	unsigned char getRecordAsUChar(char *tableName, char *columnName, char *rowNumber);
	int getRecordAsInt(char *tableName, char *columnName, char *rowNumber);
	unsigned int getRecordAsUInt(char *tableName, char *columnName, char *rowNumber);
	float getRecordAsFloat(char *tableName, char *columnName, char *rowNumber);
	bool getRecordAsBool(char *tableName, char *columnName, char *rowNumber);
	std::string getRecordAsString(char *tableName, char *columnName, char *rowNumber);

	bool searchColumnForChar(char *tableName, char *columnName, char value, std::vector<std::string> &rowList);
	bool searchColumnForUChar(char *tableName, char *columnName, unsigned char value, std::vector<std::string> &rowList);
	bool searchColumnForInt(char *tableName, char *columnName, int value, std::vector<std::string> &rowList);
	bool searchColumnForUInt(char *tableName, char *columnName, unsigned int value, std::vector<std::string> &rowList);
	bool searchColumnForFloat(char *tableName, char *columnName, float value, std::vector<std::string> &rowList);
	bool searchColumnForBool(char *tableName, char *columnName, bool value, std::vector<std::string> &rowList);
	bool searchColumnForString(char *tableName, char *columnName, std::string value, std::vector<std::string> &rowList);

	void showXML(void);

private:

	TiXmlElement* getTable(char *name);
	TiXmlElement* getColumn(char *tableName, char *columnName);
	TiXmlElement* getRecord(char *tableName, char *columnName, char *rowNumber);

	std::map<std::string, std::string> dataList;
	TiXmlDocument tinyDoc; // this will contain all the tables

};
