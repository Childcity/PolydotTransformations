#include "MetaUtils.h"

// #include <controllers/MainController.h>
// #include <enums/MeshType.h>
// #include <models/GeometryPrimitives.h>
// #include <utils/MathUtils.h>

namespace MetaUtils {

#define REG_SINGLETON(type) qmlRegisterSingletonType<type>(#type, 1, 0, #type, &type::Get)
#define REG_TYPE(type) qmlRegisterType<type *>(#type, 1, 0, #type)

template<class T>
void regEnum(const auto *name)
{
	qmlRegisterUncreatableType<T>(name, 1, 0, name, "Enum");
}

void RegisterMetaTypes()
{
	// REG_SINGLETON(MathUtils);
	// REG_TYPE(PointGeometry);
	// REG_TYPE(LineGeometry);
	// REG_TYPE(MainController);
	// qmlRegisterUncreatableType<MeshTypeClass>("MeshType", 1, 0, "MeshType", "Enum");
	// regEnum<MeshTypeClass>("MeshType");
}

} // namespace MetaUtils
