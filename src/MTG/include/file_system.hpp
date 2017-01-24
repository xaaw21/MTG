#ifndef FILE_SYSTEM_HPP
#define FILE_SYSTEM_HPP

#include <QList>

class Element
{
	QString path() const;
	QString name() const;
};

enum Filter
{
	E_FilesFilter = 0x1,
	E_DirFilter = 0x2,
	E_AllFilter = E_FilesFilter | E_DirFilter
};

class FileSystem
{
public:
	FileSystem();
	virtual ~FileSystem();

	QList<Element> entryList(Filter aFilter = E_AllFilter,const QString &aDir = QString()) const;
	uint count(Filter aFilter = E_AllFilter, const QString &aDir = QString()) const;
	bool mkdir();
	bool rmdir();

	bool rename();
	bool open(File*);
	bool close(File*);
	bool copy();
	int size(File*);
	int read(File*,char *data,int maxlen);
	int write(File*,const char *data,int len);



};

class FileEngine
{
public:

};


#endif //FILE_SYSTEM_HPP