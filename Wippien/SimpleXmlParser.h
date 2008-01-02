#ifndef _SIMPLE_XML_PARSER_H
#define _SIMPLE_XML_PARSER_H

typedef vector<void *> EntityArray;
class CXmlEntity
{
public:
	char *Name;
	char *Value;
	CXmlEntity *Parent;
	EntityArray Children;
	EntityArray Attributes;
	
	CXmlEntity()
	{
		Parent = NULL;
		Name = NULL;
		Value = NULL;
	}
	virtual ~CXmlEntity()
	{
		while (Children.size())
		{
			CXmlEntity *s = (CXmlEntity *)Children[0];
			Children.erase(Children.begin());
			delete s;
		}
		while (Attributes.size())
		{
			CXmlEntity *s = (CXmlEntity *)Attributes[0];
			Attributes.erase(Attributes.begin());
			delete s;
		}
	}
	static CXmlEntity * FindByName(CXmlEntity *start, char *name, unsigned int recursivedepth)
	{
		CXmlEntity *ret = NULL;
		if (start->Name && !strcmp(start->Name, name))
			return ret = start;
		else
		{
			if (recursivedepth)
			{
				for (int i=0;!ret && i<start->Children.size();i++)
				{
					CXmlEntity *e = (CXmlEntity *)start->Children[i];
					ret = FindByName(e, name, recursivedepth-1);
				}
			}
		}
		
		return ret;
	}
	static CXmlEntity * FindAttrByName(CXmlEntity *start, char *name)
	{
		for (int i=0;i<start->Attributes.size();i++)
		{
			CXmlEntity *e = (CXmlEntity *)start->Attributes[i];
			if (e->Name && !strcmp(name, e->Name))
				return e;
		}
		
		return NULL;
	}
	static CXmlEntity * FindByValue(CXmlEntity *start, char *value, unsigned int recursivedepth)
	{
		CXmlEntity *ret = NULL;
		if (start->Value && !strcmp(start->Value, value))
			return ret = start;
		else
		{
			if (recursivedepth)
			{
				for (int i=0;!ret && i<start->Children.size();i++)
				{
					CXmlEntity *e = (CXmlEntity *)start->Children[i];
					ret = FindByValue(e, value, recursivedepth-1);
				}
			}
		}
		
		return ret;
	}
};

#define ISWHITESPACE(X)	((X)==' ' || (X)=='\t' || (X)=='\r' || (X)=='\n' || (X)==0)
class CXmlParser  
{
public:
	CXmlParser(){};
	virtual ~CXmlParser(){};
	
	void SkipHeaders(Buffer *Data)
	{
		char *a = NULL;
		do 
		{
			a = Data->GetNextLine();
			if (a && !*a)
			{
				break;
			}
		} while(a);
	}


	char *trim(char *in)
	{
		while (*in == ' ')
			in++;
		int i = strlen(in);
		while (i>1 && in[i-1]==' ')
		{
			in[i-1] = 0;
			i--;
		}
		return in;
	}
	char *escapequotes(char *text)
	{
		if (*text == '"')
		{
			int i = strlen(text);
			if (i>0)
			{
				i--;
				if (text[i] == '"')
				{
					text[i]=0;
					return text+1;
				}
			}
		}
		
		if (*text == '\'')
		{
			int i = strlen(text);
			if (i>0)
			{
				i--;
				if (text[i] == '\'')
				{
					text[i]=0;
					return text+1;
				}
			}
		}
		
		return text;
	}
	CXmlEntity *Parse(Buffer *Data)
	{
		CXmlEntity *parent = new CXmlEntity();
		_Parse(Data, parent);
		return parent;
	}

	void CXmlParser::_ParseAttr(Buffer *Data, CXmlEntity *parent, char *attr)
	{
		BOOL inquote1 = FALSE, inquote2 = FALSE;
		char *tagstart = attr;
		char *tagname = NULL, *tagvalue = NULL;

		BOOL loopmore = FALSE;
		do
		{
			loopmore = FALSE;
			if (*attr == '\'')
				inquote1 = !inquote1;
			else
				if (*attr == '\"')
					inquote2 = !inquote2;
				else
					if (!inquote1 && !inquote2)
					{
						if (ISWHITESPACE(*attr))
						{
							if (tagname)
							{
								if (*attr)
								{
									*attr = 0;
									loopmore = TRUE;
								}
								CXmlEntity *ent = new CXmlEntity();
								ent->Parent = parent;
								ent->Name = tagname;
								if (tagvalue)
									ent->Value = escapequotes(tagvalue);
								else
									ent->Value = "";
								parent->Attributes.push_back(ent);


								tagname = NULL;
								tagvalue = NULL;
							}
						}
						else
						if (*attr=='=')
						{
							loopmore = TRUE;
							*attr = 0;
							tagvalue = attr+1;
						}
						else
							if (!tagname)
								tagname = attr;
					}

			if (*attr || loopmore)
				attr++;

		} while (*attr || loopmore);

		if (tagname)
		{
			CXmlEntity *ent = new CXmlEntity();
			ent->Parent = parent;
			ent->Name = tagname;
			if (tagvalue)
				ent->Value = escapequotes(tagvalue);
			else
				ent->Value = "";
			parent->Attributes.push_back(ent);
		}

	}

void _Parse(Buffer *Data, CXmlEntity *parent)
{
	BOOL inquote1 = FALSE, inquote2 = FALSE;
	char *tagstart = NULL;

	int strlenname = 0;
	if (parent->Name)
		strlenname = strlen(parent->Name);

	CXmlEntity *ent = new CXmlEntity();
	ent->Parent = parent;
	
	while(Data->Len())
	{
		char *d = Data->Ptr();
		Data->Consume(1);

		if (*d == '\'')
			inquote1 = !inquote1;
		else
			if (*d == '\"')
				inquote2 = !inquote2;
			else
				if (!inquote1 && !inquote2)
				{
					// not already in tag
					if (!tagstart)
					{
						if (*d == '<')
						{
							tagstart = d;
						}
					}
					else
					{
						if (*d == '>')
						{			
							char *e = tagstart+1;
							while (*e==' ')
								e++;
							if (*e=='/' && parent->Name && !strncmp(parent->Name, e+1, strlenname))
							{
								
								*tagstart = 0;
								// job done
								return;
							}
							else
							{
								// this is opening tag
								*d = 0;
								char *e = trim(tagstart+1);
								if (*e != '?')
								{
									*tagstart = 0;
									if (!ent->Name)
									{
										ent->Name = trim(e);
										ent->Value = d+1;
									}
									parent->Children.push_back(ent);

									char *attrs = NULL;
									int i = strlen(e);
									if (i>1 && e[i-1]=='/')
									{
										e[i-1] = 0;
										ent->Value = "";

										while (! ISWHITESPACE(*e))
											e++;
										if (*e)
										{
											*e = 0;
											e++;
											attrs = e;
										}
									}
									else
									{
										while (! ISWHITESPACE(*e))
											e++;
										if (*e)
										{
											*e = 0;
											e++;
											attrs = e;
										}
										_Parse(Data, ent);
									}
									if (attrs)
									{
										_ParseAttr(Data, ent, attrs);
									}


									// start new
									ent = new CXmlEntity();
									ent->Parent = parent;
								}
							}
							tagstart = NULL;
						}
					}
				}
	}
}





	

};

#endif