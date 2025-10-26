#include "../include/GameEngine/Globals.hpp"
#include "../include/GameEngine/Asset.hpp"
extern"C"
{
class KeyAssetScript
{
private:
	 float variable = 0;
;
public:
void update(sas::Asset& obj){ ;
obj.localTransform.rotation.z += 5 * sas::Globals::instance().getDeltaTime() ;
if (obj.localTransform.rotation.z >= M_PI * 2 )
obj.localTransform.rotation.z = 0 ;
} };
KeyAssetScript* create() { return new KeyAssetScript();}
void update(KeyAssetScript*instance, sas::Asset& obj){ instance->update(obj);}
void destroy(KeyAssetScript* instance) { delete instance; }
}
