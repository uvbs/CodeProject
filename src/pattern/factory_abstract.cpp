#include "factory_abstract.h"
#include "tKernel.h"

namespace factory_abstract_pattern {


WX_IMPLEMENT_DYNAMIC_VIRTUAL(IUser, GETCLASS(tNode));
WX_IMPLEMENT_DYNAMIC_VIRTUAL(IDepartment, GETCLASS(tNode));

WX_IMPLEMENT_DYNAMIC(SqlServerUser, GETCLASS(IUser));
WX_IMPLEMENT_DYNAMIC(SqlServerDepartment, GETCLASS(IDepartment));

WX_IMPLEMENT_DYNAMIC(AccessUser, GETCLASS(IUser));
WX_IMPLEMENT_DYNAMIC(AccessDepartment, GETCLASS(IDepartment));

IUser* FactoryUseReflect::createUser()
{
	std::string className = _s_dbNamePrefix + "User";
	char szName[512] = {0};
	str_util::snprintf(szName, 512, "%d", getFactoryID());
	return (IUser*)(tKernel::getSinglePtr()->newNode(className.c_str(), "bin", szName));
}

IDepartment* FactoryUseReflect::createDepartment()
{
	std::string className = _s_dbNamePrefix + "Department";
	char szName[512] = {0};
	str_util::snprintf(szName, 512, "%d", getFactoryID());
	return (IDepartment*)(tKernel::getSinglePtr()->newNode(className.c_str(), "bin", szName));
}

// 这个可从配置文件中读取
std::string FactoryUseReflect::_s_dbNamePrefix = "Access";

void test()
{
	using factory_abstract_pattern::SqlServerUser;
	using factory_abstract_pattern::SqlServerDepartment;
	using factory_abstract_pattern::AccessUser;
	using factory_abstract_pattern::AccessDepartment;
	// 注册类
	tKernel::getSinglePtr()->registerClass(GETCLASS(SqlServerUser));
	tKernel::getSinglePtr()->registerClass(GETCLASS(SqlServerDepartment));
	tKernel::getSinglePtr()->registerClass(GETCLASS(AccessUser));
	tKernel::getSinglePtr()->registerClass(GETCLASS(AccessDepartment));

	factory_abstract_pattern::User user;
	factory_abstract_pattern::Department dept;
	factory_abstract_pattern::IUser *pUser = FactoryUseReflect::createUser();
	factory_abstract_pattern::IDepartment *pDept = FactoryUseReflect::createDepartment();
	pUser->insert(&user);
	pUser->getUser(0);
	pDept->insert(&dept);
	pDept->getDepartment(0);
}

} //namespace factory_abstract_pattern
