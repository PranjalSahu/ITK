#include "itkObjectFactoryBase.h"
#include "itkDynamicLoader.h"
#include "itkDirectory.h"
#include "itkVersion.h"
#include <stdlib.h>
#include <ctype.h>
#include <algorithm>

std::list<itkObjectFactoryBase*>* itkObjectFactoryBase::m_RegisteredFactories = 0;


// Create an instance of a named itk object using the loaded
// factories

itkObject* itkObjectFactoryBase::CreateInstance(const char* itkclassname)
{
  if(!itkObjectFactoryBase::m_RegisteredFactories)
    {
    itkObjectFactoryBase::Init();
    }
  
  for(std::list<itkObjectFactoryBase*>::iterator i = m_RegisteredFactories->begin();
      i != m_RegisteredFactories->end(); ++i)
    {
    itkObject* newobject = (*i)->CreateObject(itkclassname);
    if(newobject)
      {
      return newobject;
      }
    }
  return 0;
}




// A one time initialization method.   
void itkObjectFactoryBase::Init()
{
  // Don't do anything if we are already initialized
  if(itkObjectFactoryBase::m_RegisteredFactories)
    {
    return;
    }
  
  itkObjectFactoryBase::m_RegisteredFactories =
    new std::list<itkObjectFactoryBase*>;
  itkObjectFactoryBase::RegisterDefaults();
  itkObjectFactoryBase::LoadDynamicFactories();
}


// Register any factories that are always present in ITK like
// the OpenGL factory, currently this is not done.

void itkObjectFactoryBase::RegisterDefaults()
{
}


// Load all libraries in ITK_AUTOLOAD_PATH

void itkObjectFactoryBase::LoadDynamicFactories()
{
  // follow PATH convensions
#ifdef _WIN32
  char PathSeparator = ';';
#else
  char PathSeparator = ':';
#endif
  std::string LoadPath = getenv("ITK_AUTOLOAD_PATH");
  if(LoadPath.size() == 0)
    {
    return;
    }
  std::string::size_type EndSeparatorPosition = 0;
  std::string::size_type StartSeparatorPosition = 0;
  while(StartSeparatorPosition != std::string::npos)
    {
    StartSeparatorPosition = EndSeparatorPosition;
    // find PathSeparator in LoadPath
    EndSeparatorPosition = LoadPath.find(PathSeparator, EndSeparatorPosition);
    if(EndSeparatorPosition != std::string::npos)
      {
      EndSeparatorPosition = LoadPath.size();
      }
	std::string CurrentPath = 
      LoadPath.substr(StartSeparatorPosition, EndSeparatorPosition);
    itkObjectFactoryBase::LoadLibrariesInPath(CurrentPath.c_str());
    // move past separator
    EndSeparatorPosition++;
  }
}

// A file scope helper function to concat path and file into
// a full path
static std::string CreateFullPath(const char* path, const char* file)
{
  std::string ret;
#ifdef _WIN32
  const char sep = '\\';
#else
  const char sep = '/';
#endif
  // make sure the end of path is a separator
  ret = path;
  if(ret[ret.size()-1] != sep)
    {
    ret.append(1, sep);
    }
  ret.append(file);
  return ret;
}

// A file scope typedef to make the cast code to the load
// function cleaner to read.
typedef itkObjectFactoryBase* (* ITK_LOAD_FUNCTION)();


// A file scoped function to determine if a file has
// the shared library extension in its name, this converts name to lower
// case before the compare, itkDynamicLoader always uses
// lower case for LibExtension values. 

inline bool itkNameIsSharedLibrary(const char* name)
{
  std::string sname = name;
  if(sname.find(itkDynamicLoader::LibExtension()) != std::string::npos)
    {
    return true;
    }
  return false;
}

void itkObjectFactoryBase::LoadLibrariesInPath(const char* path)
{
  itkDirectory* dir = itkDirectory::New();
  if(!dir->Load(path))
    {
    return;
    }
  
  // Attempt to load each file in the directory as a shared library
  for(int i = 0; i < dir->GetNumberOfFiles(); i++)
    {
    const char* file = dir->GetFile(i);
    // try to make sure the file has at least the extension
    // for a shared library in it.
    if(itkNameIsSharedLibrary(file))
      {
      std::string fullpath = CreateFullPath(path, file);
      itkLibHandle lib = itkDynamicLoader::OpenLibrary(fullpath.c_str());
      if(lib)
	{
	// Look for the symbol itkLoad in the library
	ITK_LOAD_FUNCTION loadfunction
	  = (ITK_LOAD_FUNCTION)itkDynamicLoader::GetSymbolAddress(lib, "itkLoad");
	// if the symbol is found call it to create the factory
	// from the library
	if(loadfunction)
	  {
	  itkObjectFactoryBase* newfactory = (*loadfunction)();
	  // initialize class members if load worked
	  newfactory->m_LibraryHandle = (void*)lib;
	  newfactory->m_LibraryPath = fullpath;
	  newfactory->m_LibraryDate = 0; // unused for now...
	  itkObjectFactoryBase::RegisterFactory(newfactory);
	  }
	}
      }
    }
}


// Recheck the ITK_AUTOLOAD_PATH for new libraries

void itkObjectFactoryBase::ReHash()
{
  itkObjectFactoryBase::UnRegisterAllFactories();
  itkObjectFactoryBase::Init();
}

// initialize class members
itkObjectFactoryBase::itkObjectFactoryBase()
{
  m_LibraryHandle = 0;
  m_LibraryDate = 0;
}


// Unload the library and free the path string
itkObjectFactoryBase::~itkObjectFactoryBase()
{
  itkDynamicLoader::CloseLibrary((itkLibHandle)m_LibraryHandle);
}

// Add a factory to the registered list
void itkObjectFactoryBase::RegisterFactory(itkObjectFactoryBase* factory)
{
  if(factory->m_LibraryHandle == 0)
    {
    const char* nonDynamicName = "Non-Dynamicly loaded factory";
    factory->m_LibraryPath = nonDynamicName;
    }
  if(strcmp(factory->GetITKSourceVersion(), 
	    itkVersion::GetITKSourceVersion()) != 0)
    {
    itkWarningMacro(<< "Possible incompatible factory load:" 
    << "\nRunning itk version :\n" << itkVersion::GetITKSourceVersion() 
    << "\nLoaded Factory version:\n" << factory->GetITKSourceVersion()
    << "\nLoading factory:\n" << factory->m_LibraryPath << "\n");
    }
  itkObjectFactoryBase::Init();
  itkObjectFactoryBase::m_RegisteredFactories->push_back(factory);
  factory->Register();
}



void itkObjectFactoryBase::PrintSelf(std::ostream& os, itkIndent indent)
{
  itkObject::PrintSelf(os, indent);
  os << indent 
     << "Factory DLL path: " << m_LibraryPath << "\n";
  os << indent << "Factory description: " << this->GetDescription() << endl;
  int num = m_OverrideMap.size();
  os << indent << "Factory overides " << num << " classes:" << endl;
  indent = indent.GetNextIndent();
  for(itkObjectFactoryBase::OverRideMap::iterator i = m_OverrideMap.begin();
      i != m_OverrideMap.end(); ++i)
    {
    os << indent << "Class : " <<  (*i).first 
       << std::endl;
    os << indent << "Overriden with: " <<  (*i).second.m_OverrideWithName
       << std::endl;
    os << indent << "Enable flag: " << (*i).second.m_EnabledFlag  
       << std::endl;
    os << std::endl;
  }
}

void itkObjectFactoryBase::UnRegisterFactory(itkObjectFactoryBase* factory)
{ 
  for(std::list<itkObjectFactoryBase*>::iterator i = 
	m_RegisteredFactories->begin();
      i != m_RegisteredFactories->end();
      ++i)
    {
    if(factory == *i)
      {
      m_RegisteredFactories->remove(factory);
      factory->UnRegister();
      return;
      }
    }
}
  

// unregister all factories and delete the RegisteredFactories list
void itkObjectFactoryBase::UnRegisterAllFactories()
{
  itkObjectFactoryBase* factory = 0; 
  for(std::list<itkObjectFactoryBase*>::iterator i 
	= m_RegisteredFactories->begin();
      i != m_RegisteredFactories->end(); ++i)
    {
    (*i)->UnRegister();
    }
  delete itkObjectFactoryBase::m_RegisteredFactories;
  itkObjectFactoryBase::m_RegisteredFactories = 0;
}



void itkObjectFactoryBase::RegisterOverride(const char* classOverride,
					const char* subclass,
					const char* description,
					bool enableFlag,
					itkCreateObjectFunctionBase*
					    createFunction)
{
  itkObjectFactoryBase::OverrideInformation info;
  info.m_Description = description;
  info.m_OverrideWithName = subclass;
  info.m_EnabledFlag = enableFlag;
  info.m_CreateObject = createFunction;
  m_OverrideMap.insert(OverRideMap::value_type(classOverride, info));
}


itkObject* itkObjectFactoryBase::CreateObject(const char* itkclassname)
{
  m_OverrideMap.find(itkclassname);
  OverRideMap::iterator pos = m_OverrideMap.find(itkclassname);
  if(pos != m_OverrideMap.end())
    {
    return (*pos).second.m_CreateObject->CreateObject();
    }
}


void itkObjectFactoryBase::SetEnableFlag(bool flag,
					 const char* className,
					 const char* subclassName)
{
  OverRideMap::iterator start = m_OverrideMap.lower_bound(className);
  OverRideMap::iterator end = m_OverrideMap.upper_bound(className);
  for(OverRideMap::iterator i = start; i != end; ++i)
    {
    if((*i).second.m_OverrideWithName == subclassName)
      {
      (*i).second.m_EnabledFlag = flag;
      }
    }
}

bool itkObjectFactoryBase::GetEnableFlag(const char* className,
					 const char* subclassName)
{
  OverRideMap::iterator start = m_OverrideMap.lower_bound(className);
  OverRideMap::iterator end = m_OverrideMap.upper_bound(className);
  for(OverRideMap::iterator i = start; i != end; ++i)
    {
    if((*i).second.m_OverrideWithName == subclassName)
      {
      return (*i).second.m_EnabledFlag;
      }
    }
  return 0;
}



void itkObjectFactoryBase::Disable(const char* className)
{
  OverRideMap::iterator start = m_OverrideMap.lower_bound(className);
  OverRideMap::iterator end = m_OverrideMap.upper_bound(className);
  for(OverRideMap::iterator i = start; i != end; ++i)
    {
    (*i).second.m_EnabledFlag = 0;
    }
}


std::list<itkObjectFactoryBase*> itkObjectFactoryBase::GetRegisteredFactories()
{
  return *itkObjectFactoryBase::m_RegisteredFactories;
}

// Return a list of classes that this factory overrides.
std::list<std::string> itkObjectFactoryBase::GetClassOverrideNames()
{
  std::list<std::string> ret;
  for(OverRideMap::iterator i = m_OverrideMap.begin();
      i != m_OverrideMap.end(); ++i)
    {
    ret.push_back((*i).first);
    }
  return ret;
}

// Return a list of the names of classes that override classes.
std::list<std::string> itkObjectFactoryBase::GetClassOverrideWithNames()
{
  std::list<std::string> ret;
  for(OverRideMap::iterator i = m_OverrideMap.begin();
      i != m_OverrideMap.end(); ++i)
    {
    ret.push_back((*i).second.m_OverrideWithName);
    }
  return ret;
}

// Retrun a list of descriptions for class overrides
std::list<std::string> itkObjectFactoryBase::GetClassOverrideDescriptions()
{ 
  std::list<std::string> ret;
  for(OverRideMap::iterator i = m_OverrideMap.begin();
      i != m_OverrideMap.end(); ++i)
    {
    ret.push_back((*i).second.m_Description);
    }
  return ret;
}

// Return a list of enable flags
std::list<bool> itkObjectFactoryBase::GetEnableFlags()
{
  std::list<bool> ret;
  for(OverRideMap::iterator i = m_OverrideMap.begin();
      i != m_OverrideMap.end(); ++i)
    {
    ret.push_back((*i).second.m_EnabledFlag);
    }
  return ret;
}

