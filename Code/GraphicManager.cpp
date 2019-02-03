#include "GraphicManager.h"

GraphicManager *Singleton<GraphicManager>::sInstance = 0;

#include <iostream>

using namespace std;

GraphicManager::GraphicManager()
{
	device = 0;
	env = 0;
	driver = 0;
	smgr = 0;

	oldTime = 0;

	bgR = 0;
	bgB = 0;
	bgG = 0;
	bgA = 0;

	camera = 0;

	ID = 0;

	oldMouseX = 0;
	oldMouseY = 0;

}

GraphicManager::~GraphicManager()
{
	device->drop();
}

void GraphicManager::run(unsigned int videoType, unsigned int width, unsigned int height, unsigned int bits, bool fullScreen, bool stencilBuffer, bool vsync, IEventReceiver *receiver)
{
	device = createDevice((irr::video::E_DRIVER_TYPE)videoType /*video::EDT_OPENGL*/ /*for now*/, dimension2d<u32>(width, height), bits, fullScreen, stencilBuffer, vsync, receiver);

	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	env = device->getGUIEnvironment();
	/*SKeyMap keymap[6];
	keymap[0].Action = irr::EKA_MOVE_FORWARD;
	keymap[0].KeyCode = irr::KEY_KEY_W;
	keymap[1].Action = irr::EKA_STRAFE_LEFT;
	keymap[1].KeyCode = irr::KEY_KEY_A;
	keymap[2].Action = irr::EKA_STRAFE_RIGHT;
	keymap[2].KeyCode = irr::KEY_KEY_D;
	keymap[3].Action = irr::EKA_MOVE_BACKWARD;
	keymap[3].KeyCode = irr::KEY_KEY_S;
	keymap[4].Action = irr::EKA_CROUCH;
	keymap[4].KeyCode = irr::KEY_KEY_F;
	keymap[5].Action = irr::EKA_JUMP_UP;
	keymap[5].KeyCode = irr::KEY_KEY_J;
	
	camera = smgr->addCameraSceneNodeFPS(0,100.0f,0.6f,-1,keymap,6,true);*/
	camera = smgr->addCameraSceneNode();
}
void GraphicManager::SetWindowCaption(wchar_t *name) {
	device->setWindowCaption(name);
}

float GraphicManager::getNewDeltaTime(void)
{
	unsigned int currentTime = device->getTimer()->getRealTime();
	float dt = (currentTime - oldTime) / 1000.0;
	oldTime = currentTime;

	return dt;
}

void GraphicManager::getMousePosition(int &x, int &y)
{
	core::position2d<s32> pos = device->getCursorControl()->getPosition();

	x = pos.X;
	y = pos.Y;
}

void GraphicManager::getDeltaMousePosition(int &x, int &y)
{
	int newX, newY;
	int dX, dY;

	getMousePosition(newX, newY);

	dX = newX - oldMouseX;
	dY = newY - oldMouseY;

	oldMouseX = newX;
	oldMouseY = newY;

	x = dX;
	y = dY;
}

void GraphicManager::setCameraPosition(float x, float y, float z)
{
	camera->setPosition(vector3df(x, y, z));
}

void GraphicManager::setCameraLookAt(float x, float y, float z)
{
	camera->setTarget(vector3df(x, y, z));
}

void GraphicManager::setBackgroundColor(unsigned int r, unsigned int b, unsigned int g, unsigned int a)
{
	bgR = r;
	bgB = b;
	bgG = g;
	bgA = a;
}

void GraphicManager::setFont(char *fontFile)
{
	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont(fontFile);

	if (font)
		skin->setFont(font);
}

bool GraphicManager::doesGUIElementExists(unsigned int id)
{
	return (id2DList.find(id) != id2DList.end());
}

IGUIElement *GraphicManager::getElementFromId(unsigned int id)
{
	return env->getRootGUIElement()->getElementFromId(id, true); // true for check all childern also;
}

unsigned int GraphicManager::createButton(float startX, float startY, float endX, float endY)
{
	ID++;
	id2DList[ID] = env->addButton(rect<s32>(startX, startY, endX, endY), 0, ID);

	return ID;
}

unsigned int GraphicManager::createEditBox(float startX, float startY, float endX, float endY)
{
	ID++;
	id2DList[ID] = env->addEditBox(L"", rect<s32>(startX, startY, endX, endY), true, 0, ID);

	return ID;
}

void GraphicManager::setEditBoxPasswordBox(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		IGUIEditBox *elme = (IGUIEditBox *)getElementFromId(id);

		elme->setPasswordBox(true);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setEditBoxTextAlignment(unsigned int id, unsigned int horizontal, unsigned int vertical)
{
	if(doesGUIElementExists(id))
	{
		IGUIEditBox *elme = (IGUIEditBox *)getElementFromId(id);

		elme->setTextAlignment((irr::gui::EGUI_ALIGNMENT)horizontal, (irr::gui::EGUI_ALIGNMENT)vertical);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setEditBoxAutoScroll(unsigned int id, bool scroll)
{

	if(doesGUIElementExists(id))
	{
		IGUIEditBox *elme = (IGUIEditBox *)getElementFromId(id);

		elme->setAutoScroll(scroll);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setEditBoxWordWrap(unsigned int id, bool wrap)
{
	if(doesGUIElementExists(id))
	{
		IGUIEditBox *elme = (IGUIEditBox *)getElementFromId(id);

		elme->setWordWrap(wrap);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setEditBoxMultiLine(unsigned int id, bool mLine)
{
	if(doesGUIElementExists(id))
	{
		IGUIEditBox *elme = (IGUIEditBox *)getElementFromId(id);

		elme->setMultiLine(mLine);
	}
	else
	{
		// throw("something");
	}
}

unsigned int GraphicManager::createScrollBar(bool horizontal, float startX, float startY, float endX, float endY)
{
	ID++;
	id2DList[ID] = env->addScrollBar(horizontal, rect<s32>(startX, startY, endX, endY), 0, ID);

	return ID;
}

void GraphicManager::setScrollBarMax(unsigned int id, int value)
{
	if(doesGUIElementExists(id))
	{
		IGUIScrollBar *elme = (IGUIScrollBar*)getElementFromId(id);

		elme->setMax(value);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setScrollBarPosition(unsigned int id, int value)
{
	if(doesGUIElementExists(id))
	{
		IGUIScrollBar *elme = (IGUIScrollBar*)getElementFromId(id);

		elme->setPos(value);
	}
	else
	{
		// throw("something");
	}
}

int GraphicManager::getScrollBarMax(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		IGUIScrollBar *elme = (IGUIScrollBar*)getElementFromId(id);

		return elme->getMax();
	}
	else
	{
		// throw("something");
	}

	return 0;
}

int GraphicManager::getScrollBarPosition(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		IGUIScrollBar *elme = (IGUIScrollBar*)getElementFromId(id);

		return elme->getPos();
	}
	else
	{
		// throw("something");
	}

	return 0;
}

unsigned int GraphicManager::createStaticText(float startX, float startY, float endX, float endY)
{
	ID++;
	id2DList[ID] = env->addStaticText(L"", rect<s32>(startX, startY, endX, endY), true, true, 0, ID, false);

	return ID;
}

void GraphicManager::setStaticTextBackgroundColor(unsigned int id, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	if(doesGUIElementExists(id))
	{
		IGUIStaticText *elme = (IGUIStaticText*)getElementFromId(id);

		elme->setBackgroundColor(SColor(a, r, g, b));
	}
	else
	{
		// throw("something");
	}
}

unsigned int GraphicManager::createMessageBox(wchar_t *caption, wchar_t *body, int flags)
{
	ID++;
	id2DList[ID] = env->addMessageBox(caption, body, true, flags, 0, ID);

	return ID;
}

unsigned int GraphicManager::createImage(char *fileName, float x, float y)
{
	ID++;
	id2DList[ID] = env->addImage(driver->getTexture(fileName), position2d<s32>(x, y), true, 0, ID, 0);

	return ID;
}

void GraphicManager::setImageColor(unsigned int id, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	if(doesGUIElementExists(id))
	{
		IGUIImage *elme = (IGUIImage*)getElementFromId(id);

		elme->setColor(SColor(a, r, g, b));
	}
	else
	{
		// throw("something");
	}
}

unsigned int GraphicManager::createInOutFader(float startX, float startY, float endX, float endY)
{
	ID++;
	rect<s32> *box = new rect<s32>(startX, startY, endX, endY);
	id2DList[ID] = env->addInOutFader(box, 0, ID);
	delete box;

	return ID;
}

void GraphicManager::setInOutFaderFadeIn(unsigned int id, unsigned int value)
{
	if(doesGUIElementExists(id))
	{
		IGUIInOutFader *elme = (IGUIInOutFader*)getElementFromId(id);

		elme->fadeIn(value);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setInOutFaderFadeOut(unsigned int id, unsigned int value)
{
	if(doesGUIElementExists(id))
	{
		IGUIInOutFader *elme = (IGUIInOutFader*)getElementFromId(id);

		elme->fadeOut(value);
	}
	else
	{
		// throw("something");
	}
}

bool GraphicManager::InOutFaderIsReady(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		IGUIInOutFader *elme = (IGUIInOutFader*)getElementFromId(id);

		return elme->isReady();
	}
	else
	{
		// throw("something");
	}

	return false;
}

void GraphicManager::setInOutFaderColor(unsigned int id, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
{
	if(doesGUIElementExists(id))
	{
		IGUIInOutFader *elme = (IGUIInOutFader*)getElementFromId(id);

		elme->setColor(SColor(a, r, g, b));
	}
	else
	{
		// throw("something");
	}
}

unsigned int GraphicManager::createSpinBox(float startX, float startY, float endX, float endY)
{
	ID++;
	id2DList[ID] = env->addSpinBox(L"", rect<s32>(startX, startY, endX, endY), true, 0, ID);

	return ID;
}

float GraphicManager::getSpinBoxValue(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		IGUISpinBox *elme = (IGUISpinBox*)getElementFromId(id);

		return elme->getValue();
	}
	else
	{
		// throw("something");
	}

	return 0;
}

void GraphicManager::setSpinBoxRange(unsigned int id, float min, float max, float step)
{
	if(doesGUIElementExists(id))
	{
		IGUISpinBox *elme = (IGUISpinBox*)getElementFromId(id);

		elme->setRange(min, max);
		elme->setStepSize(step);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setSpinBoxDecimalPlaces(unsigned int id, unsigned int dp)
{
	if(doesGUIElementExists(id))
	{
		IGUISpinBox *elme = (IGUISpinBox*)getElementFromId(id);

		elme->setDecimalPlaces(dp);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setGUIPosition(unsigned int id, float x, float y, float width, float height)
{
	// always check if element exist, or throw an exctiption
	if(doesGUIElementExists(id))
	{
		IGUIElement *elme = getElementFromId(id);

		rect<s32> box = rect<s32>(x, y, x + width, y + height);
		elme->setRelativePosition(box);
	}
	else
	{
		// throw("something");
	}
}

//void GraphicManager::setGUIMaxSize(unsigned int id, float width, float height)
//{
//}
//
//void GraphicManager::setGUIMinSize(unsigned int id, float width, float height)
//{
//}
//
//void GraphicManager::setGUISize(unsigned int id, float width, float height)
//{
//}

void GraphicManager::setGUIText(unsigned int id, const wchar_t *text)
{
	if(doesGUIElementExists(id))
	{
		IGUIElement *elme = getElementFromId(id);

		elme->setText(text);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setGUIText(unsigned int id, char* text)
{
	if(doesGUIElementExists(id))
	{
		IGUIElement *elme = getElementFromId(id);

		elme->setText(irr::core::stringw(irr::core::stringc(text)).c_str());
	}
	else
	{
		// throw("something");
	}
}

wchar_t* GraphicManager::getGUIText(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		IGUIElement *elme = getElementFromId(id);

		return (wchar_t*)elme->getText();
	}
	else
	{
		// throw("something");
	}

	return 0;
}

void GraphicManager::setGUIChild(unsigned int id, unsigned int childID)
{
	if(doesGUIElementExists(id) && doesGUIElementExists(childID))
	{
		IGUIElement *elme = getElementFromId(id);

		elme->addChild(getElementFromId(childID));
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::setGUIFocus(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		IGUIElement *elme = getElementFromId(id);

		env->setFocus(elme);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::removeGUIFocus(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		IGUIElement *elme = getElementFromId(id);

		env->removeFocus(elme);
	}
	else
	{
		// throw("something");
	}
}

bool GraphicManager::hasGUIFocus(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		IGUIElement *elme = getElementFromId(id);

		return env->hasFocus(elme);
	}
	else
	{
		// throw("something");

		return false;
	}
}

void GraphicManager::removeGUIElement(unsigned int id)
{
	if(doesGUIElementExists(id))
	{
		// we check first if the element hasn't been deleted from the Irrlicht side
		// this could happen if you remove a message box or a window

		if(env->getRootGUIElement()->getElementFromId(id, true)) // we check if Irrlicht still has this element
		{
			IGUIElement *elme = getElementFromId(id);

			elme->remove();
		}

		id2DList.erase(id);
	}
	else
	{
		// throw("something");
	}
}

void GraphicManager::sendGUIEvent(unsigned int callerID, unsigned int elementID, unsigned int eventID)
{
	IGUIElement *caller = 0;
	IGUIElement *element = 0;
	irr::SEvent evt;
	irr::SEvent::SGUIEvent guiEvt;

	if(doesGUIElementExists(callerID))
	{
		caller = getElementFromId(callerID);
	}

	if(doesGUIElementExists(elementID))
	{
		element = getElementFromId(elementID);

		guiEvt.Caller = caller;
		guiEvt.Element = element;

		guiEvt.EventType = irr::gui::EGUI_EVENT_TYPE(eventID);

		evt.GUIEvent = guiEvt;

		env->postEventFromUser(evt);
	}
	else
	{
		// if there is no reciver then there is no need to continue
	}
}

void GraphicManager::tick(void)
{
	device->run();
}

void GraphicManager::drawAll(void)
{
	driver->beginScene(true, true, video::SColor(bgR, bgB, bgG,bgA));

	smgr->drawAll();
	env->drawAll();

	driver->endScene();
}

//void GraphicManager::startDraw(void)
//{
//	device->run();
//	driver->beginScene(true, true, video::SColor(bgR, bgB, bgG,bgA));
//}

//void GraphicManager::endDraw(void)
//{
//	driver->endScene();
//}

void GraphicManager::clearGUI(void)
{
	env->clear();
}

void GraphicManager::clearSceneManager(void)
{
	smgr->clear();
	// don't forget to make another camera
	camera = smgr->addCameraSceneNode();
}

bool GraphicManager::doesNodeExists(unsigned int id)
{
	return (id3DList.find(id) != id3DList.end());
}

ISceneNode *GraphicManager::getNodeFromId(unsigned int id)
{
	return id3DList[id];
}

unsigned int GraphicManager::createTestBox(unsigned int r, unsigned int g, unsigned int b)
{
	ID++;
	id3DList[ID] = smgr->addCubeSceneNode();
	id3DList[ID]->setMaterialFlag(EMF_LIGHTING, true);

	// change color
	IMeshSceneNode *cube = (IMeshSceneNode*)id3DList[ID];
	smgr->getMeshManipulator()->setVertexColors(cube->getMesh(), SColor(255, r, g, b));

	return ID;
}

unsigned int GraphicManager::createAnimatedMesh(char *meshFileName)
{
	ID++;

	IAnimatedMeshSceneNode *node = smgr->addAnimatedMeshSceneNode(smgr->getMesh(meshFileName));
	node->setMaterialFlag(EMF_LIGHTING, false);

	id3DList[ID] = node;

	return ID;
}

void GraphicManager::setAnimationFrameLoop(unsigned int id, unsigned int start, unsigned int end)
{
	if(doesNodeExists(id))
	{
		IAnimatedMeshSceneNode *node = (IAnimatedMeshSceneNode*)getNodeFromId(id);

		node->setFrameLoop(start, end);
	}
	else
	{
		// throw something
	}
}

void GraphicManager::setAnimationCurrentFrame(unsigned int id, unsigned int frameCount)
{
	if(doesNodeExists(id))
	{
		IAnimatedMeshSceneNode *node = (IAnimatedMeshSceneNode*)getNodeFromId(id);

		node->setCurrentFrame(frameCount);
	}
	else
	{
		// throw something
	}
}

unsigned int GraphicManager::getAnimationCurrentFrame(unsigned int id)
{
	if(doesNodeExists(id))
	{
		IAnimatedMeshSceneNode *node = (IAnimatedMeshSceneNode*)getNodeFromId(id);

		return node->getFrameNr();
	}
	else
	{
		// throw something
	}

	return 0;
}

void GraphicManager::setAnimationFrameSpeed(unsigned int id, float speed)
{
	if(doesNodeExists(id))
	{
		IAnimatedMeshSceneNode *node = (IAnimatedMeshSceneNode*)getNodeFromId(id);

		node->setAnimationSpeed(speed);
	}
	else
	{
		// throw something
	}
}

void GraphicManager::setAnimationLoop(unsigned int id, bool loop)
{
	if(doesNodeExists(id))
	{
		IAnimatedMeshSceneNode *node = (IAnimatedMeshSceneNode*)getNodeFromId(id);

		node->setLoopMode(loop);
	}
	else
	{
		// throw something
	}
}

void GraphicManager::setAnimationTransitionTime(unsigned int id, float time)
{
	if(doesNodeExists(id))
	{
		IAnimatedMeshSceneNode *node = (IAnimatedMeshSceneNode*)getNodeFromId(id);

		node->setTransitionTime(time);
	}
	else
	{
		// throw something
	}
}

unsigned int GraphicManager::create3DText(char *fontName, char *text)
{
	ID++;

	//ITextSceneNode *node = smgr->addTextSceneNode(/*gui::IGUIFont *font*/ 0, (const wchar_t*) text, SColor(100, 255, 255, 255), 0, vector3df(0, 0, 0), ID);
	//ITextSceneNode *node = smgr->addTextSceneNode(env->getBuiltInFont(), L"test name", SColor(100, 255, 255, 255), 0, vector3df(0, 20, 0), ID);
	//ITextSceneNode *node = smgr->addTextSceneNode(env->getFont(fontName), irr::core::stringw(text).c_str(), SColor(100, 255, 255, 255), 0, vector3df(0, 20, 0), ID);
	ITextSceneNode *node = smgr->addTextSceneNode(env->getBuiltInFont(), irr::core::stringw(text).c_str(), SColor(100, 255, 255, 255), 0, vector3df(0, 0, 0), ID);

	id3DList[ID] = node;

	return ID;
}

unsigned int GraphicManager::createParticle(void)
{
	ID++;

	IParticleSystemSceneNode *node =smgr->addParticleSystemSceneNode(false);

	id3DList[ID] = node;

 	return ID;
}

void GraphicManager::createParticlePointEmitter(unsigned int id, float dirX, float dirY, float dirZ, unsigned int minParticlesPerSecond, unsigned int maxParticlesPerSecond, unsigned int minR, unsigned int minG, unsigned int minB, unsigned int minA, unsigned int maxR, unsigned int maxG, unsigned int maxB, unsigned int maxA, unsigned int lifeTimeMin, unsigned int lifeTimeMax, int maxAngleDegrees, float minX, float minY, float maxX, float maxY)
{
	if(doesNodeExists(id))
	{
		IParticleSystemSceneNode *node = (IParticleSystemSceneNode*)getNodeFromId(id);

		IParticleEmitter* em = node->createPointEmitter(vector3df(dirX, dirY, dirZ), minParticlesPerSecond, maxParticlesPerSecond, SColor(minA, minR, minG, minB), SColor(maxA, maxR, maxG, maxB), lifeTimeMin, lifeTimeMax, maxAngleDegrees, dimension2df(minX, minY), dimension2df(maxX, maxY));

		node->setEmitter(em);
		em->drop();
	}
	else
	{
		// throw something
	}
}

void GraphicManager::addParticleFadeOutAffector(unsigned int id, unsigned int r, unsigned int g, unsigned int b, unsigned int a, unsigned int timeNeededToFadeOut)
{
	if(doesNodeExists(id))
	{
		IParticleSystemSceneNode *node = (IParticleSystemSceneNode*)getNodeFromId(id);

		IParticleAffector* paf = node->createFadeOutParticleAffector(SColor(a, r, g, b), timeNeededToFadeOut);

		node->addAffector(paf);
		paf->drop();
	}
	else
	{
		// throw something
	}
}

void GraphicManager::addParticleGravityAffector(unsigned int id, float gravityX, float gravityY, float gravityZ, unsigned int timeForceLost)
{
	if(doesNodeExists(id))
	{
		IParticleSystemSceneNode *node = (IParticleSystemSceneNode*)getNodeFromId(id);

		IParticleAffector* paf = node->createGravityAffector(vector3df(gravityX, gravityY, gravityZ), timeForceLost);

		node->addAffector(paf);
		paf->drop();
	}
	else
	{
		// throw something
	}
}

unsigned int GraphicManager::createBillboard(void)
{
	ID++;

	IBillboardSceneNode *node =smgr->addBillboardSceneNode(0, dimension2d<f32>(0.0f, 0.0f), vector3df(0, 0, 0), -1, 0xFFFFFFFF, 0xFFFFFFFF);
;
	id3DList[ID] = node;

 	return ID;
}

void GraphicManager::setBillboardSize(unsigned int id, float x, float y)
{
	if(doesNodeExists(id))
	{
		IBillboardSceneNode *node = (IBillboardSceneNode*)getNodeFromId(id);

		node->setSize(dimension2d<f32>(x, y));
	}
	else
	{
		// throw something
	}
}

void GraphicManager::setBillBoardColor(unsigned int id, unsigned int topR, unsigned int topG, unsigned int topB, unsigned int topA, unsigned int bottomR, unsigned int bottomG, unsigned int bottomB, unsigned int bottomA)
{
	if(doesNodeExists(id))
	{
		IBillboardSceneNode *node = (IBillboardSceneNode*)getNodeFromId(id);

		node->setColor(SColor(topA, topR, topG, topB), SColor(bottomA, bottomR, bottomG, bottomB));
	}
	else
	{
		// throw something
	}
}

void GraphicManager::setNodePosition(unsigned int id, float x, float y, float z)
{
	if(doesNodeExists(id))
	{
		getNodeFromId(id)->setPosition(vector3df(x, y, z));
	}
	else
	{
		// there was a problem. We couldn't find an object with the same ID
		throw("Error from GraphicManager: Object ID = xxx doesn't exists");
	}
}

void GraphicManager::setNodeRotation(unsigned int id, float x, float y, float z)
{
	if(doesNodeExists(id))
	{
		getNodeFromId(id)->setRotation(vector3df(x, y, z));
	}
	else
	{
		// there was a problem. We couldn't find an object with the same ID
		throw("Error from GraphicManager: Object ID = xxx doesn't exists");
	}
}

void GraphicManager::setNodeScale(unsigned int id, float x, float y, float z)
{
	if(doesNodeExists(id))
	{
		getNodeFromId(id)->setScale(vector3df(x, y, z));
	}
	else
	{
		// there was a problem. We couldn't find an object with the same ID
		throw("Error from GraphicManager: Object ID = xxx doesn't exists");
	}
}

void GraphicManager::setNodeParent(unsigned int id, unsigned int childID)
{
	if(doesNodeExists(id) && doesNodeExists(childID))
	{
		getNodeFromId(childID)->setParent(getNodeFromId(id));
	}
	else
	{
		// there was a problem. We couldn't find an object with the same ID
		throw("Error from GraphicManager: Object ID = xxx doesn't exists");
	}
}

void GraphicManager::removeNode(unsigned int id)
{
	if(doesNodeExists(id))
	{
		// get the scene node
		ISceneNode *node = getNodeFromId(id);

		node->remove();

		id3DList.erase(id);
	}
	else
	{
		// there was a problem. We couldn't find an object with the same ID
		throw("Error from GraphicManager: Object ID = xxx doesn't exists");
	}
}

void GraphicManager::setNodeTexture(unsigned int id, unsigned int textureLayer, char *fileName)
{
	if(doesNodeExists(id))
	{
		ISceneNode *node = getNodeFromId(id);

		node->setMaterialTexture(textureLayer, driver->getTexture(fileName));
	}
	else
	{
		// throw something
	}
}

void GraphicManager::setNodeMaterialType(unsigned int id, unsigned int type)
{
	if(doesNodeExists(id))
	{
		ISceneNode *node = getNodeFromId(id);

		node->setMaterialType((irr::video::E_MATERIAL_TYPE)type);
	}
	else
	{
		// throw something
	}
}

void GraphicManager::setNodeMaterialFlag(unsigned int id, unsigned int flag, bool isOn)
{
	if(doesNodeExists(id))
	{
		ISceneNode *node = getNodeFromId(id);

		node->setMaterialFlag((irr::video::E_MATERIAL_FLAG)flag, isOn);
	}
	else
	{
		// throw something
	}
}

void GraphicManager::normalizeVector(float &x, float &y, float &z)
{
	vector3df vec(x, y, z);

	vec.normalize();

	x = vec.X;
	y = vec.Y;
	z = vec.Z;
}

void GraphicManager::crossProductVector(float &forwardX, float &forwardY, float &forwardZ, float upX, float upY, float upZ)
{
	vector3df forwardVec(forwardX, forwardY, forwardZ);
	vector3df crossVec = forwardVec.crossProduct(vector3df(upX, upY, upZ));

	// we put the values in the arguenets to be sent back
	forwardX = crossVec.X;
	forwardY = crossVec.Y;
	forwardZ = crossVec.Z;
}

void GraphicManager::directionToRotation(float &x, float &y, float &z)
{
	vector3df direction(x, y, z);
	vector3df rotation = direction.getHorizontalAngle();

	// we put the values in the arguenets to be sent back
	x = rotation.X;
	y = rotation.Y;
	z = rotation.Z;
}

void GraphicManager::rotationToDirection(float &x, float &y, float &z)
{
	vector3df rotation(x, y, z);
	vector3df direction = rotation.rotationToDirection();

	// we put the values in the arguenets to be sent back
	x = direction.X;
	y = direction.Y;
	z = direction.Z;
}

float GraphicManager::vectorLength(float x, float y, float z)
{
	return vector3df(x, y, z).getLength();
}

unsigned int GraphicManager::loadmap(irr::io::path modelname, unsigned int id)
{
	smgr->loadScene(modelname);
	irr::scene::ILightSceneNode* light = smgr->addLightSceneNode(0,irr::core::vector3df(0,50,0),irr::video::SColorf(1.00,1.00,1.00),300);

	light->setRotation(vector3df(30, 0, 0));

	light->setLightType(irr::video::E_LIGHT_TYPE::ELT_DIRECTIONAL);
	light->enableCastShadow(true);

	scene::IMetaTriangleSelector * meta = smgr->createMetaTriangleSelector();

	core::array<scene::ISceneNode *> nodes;
	smgr->getSceneNodesFromType(scene::ESNT_ANY, nodes); // Find all nodes

	for (u32 i=0; i < nodes.size(); ++i)
	{
		scene::ISceneNode * node = nodes[i];
		scene::ITriangleSelector * selector = 0;

		switch(node->getType())
		{
		case scene::ESNT_CUBE:
		case scene::ESNT_ANIMATED_MESH:
			// Because the selector won't animate with the mesh,
			// and is only being used for camera collision, we'll just use an approximate
			// bounding box instead of ((scene::IAnimatedMeshSceneNode*)node)->getMesh(0)
			selector = smgr->createTriangleSelectorFromBoundingBox(node);
			break;

		case scene::ESNT_MESH:
		case scene::ESNT_SPHERE: // Derived from IMeshSceneNode
			selector = smgr->createTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);
			break;

		case scene::ESNT_TERRAIN:
			selector = smgr->createTerrainTriangleSelector((scene::ITerrainSceneNode*)node);
			break;

		case scene::ESNT_OCTREE:
			selector = smgr->createOctreeTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);
			break;

		default:
			// Don't create a selector for this node type
			break;
		}

		if(selector)
		{
			// Add it to the meta selector, which will take a reference to it
			meta->addTriangleSelector(selector);
			// And drop my reference to it, so that the meta selector owns it.
			selector->drop();
		}
	}
				
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(meta, camera, core::vector3df(5,5,5),core::vector3df(0,0,0));
	meta->drop(); // I'm done with the meta selector now

	camera->addAnimator(anim);
	anim->drop(); // I'm done with the animator now
	return 0;
}

void GraphicManager::CreateDefaultSkyBox()
{
	smgr->addSkyDomeSceneNode(driver->getTexture("Sky.jpg"));
}

unsigned int GraphicManager::MetaSelector() {
	ID++;
	idTSList[ID] = smgr->createMetaTriangleSelector();
	return ID;
}


void GraphicManager::AddAnimatorToMesh(unsigned int MetaID, unsigned int MeshID, unsigned int animatr) {
	if(idTSList.find(MetaID) != idTSList.end()) {
		if(idAList.find(animatr) != idAList.end()) {
			if(doesNodeExists(MeshID)) {
				ISceneNode* node = getNodeFromId(MeshID);
				IMetaTriangleSelector* imts = idTSList[MetaID];
				idAList[animatr] = smgr->createCollisionResponseAnimator(imts, node, core::vector3df(30,50,30),
							core::vector3df(0,-10,0), core::vector3df(0,30,0));
				node->addAnimator(idAList[animatr]);
				
			}
		}
	}
}

unsigned int GraphicManager::NodeAnimator() {
	ID++;
	ISceneNodeAnimator* an;
	idAList[ID] = an;
	return ID;
}

unsigned int GraphicManager::DrawInventory(unsigned int id) {
	ID++;
	gui::IGUIWindow* win = env->addWindow(core::rect<s32>(320, 90, 550, 470));
	id2DList[ID] = win;
	id2DList[ID]->addChild(id2DList[id]);
	return ID;
}

unsigned int GraphicManager::CreateImagegui(unsigned int InventoryID) {
	if(idTexList.find(InventoryID) != idTexList.end()) {
	ID++;
	id2DList[ID] = env->addImage(idTexList[ImageID],core::vector2d<s32>(400,400),true,id2DList[InventoryID]);
	}
	return ID;
}

unsigned int GraphicManager::CreateTexture(char* filename) {
	ID++;
	idTexList[ID] = driver->getTexture(filename);
	return ID;
}

/*unsigned int GraphicManager::SetTexture(unsigned int id) {
	return idTexList[id];	
}*/


float GraphicManager::getNodeX(unsigned int idm) {
	if(doesNodeExists(idm)) 
		return id3DList[idm]->getPosition().X;
}
float GraphicManager::getNodeY(unsigned int idm) {
	if(doesNodeExists(idm)) 
		return id3DList[idm]->getPosition().Y;
}
float GraphicManager::getNodeZ(unsigned int idm) {
	if(doesNodeExists(idm)) 
		return id3DList[idm]->getPosition().Z;
}

/*GraphicManager::Wait(int seconds){
	clock_t endwait;
  endwait = clock () + seconds * CLOCKS_PER_SEC ;
  while (clock() < endwait) {}
}*/

void GraphicManager::InvVisible(bool truefalse, unsigned int invid){
	if(doesGUIElementExists(invid)) {
		id2DList[invid]->setVisible(truefalse);
	}
		
}



