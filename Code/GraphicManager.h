#pragma once

#include "Singleton.h"
#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#include <map>

class GraphicManager : public Singleton<GraphicManager>
{

public:

	GraphicManager();
	~GraphicManager();
	void run(unsigned int videoType, unsigned int width, unsigned int height, unsigned int bits, bool fullScreen, bool stencilBuffer, bool vsync, IEventReceiver *receiver = 0);
	void SetWindowCaption(wchar_t *name);
	float getNewDeltaTime(void);

	void getMousePosition(int &x, int &y); // mouse position
	void getDeltaMousePosition(int &x, int &y); // get the differnce between the old position and the new one

	void setCameraPosition(float x, float y, float z);
	void setCameraLookAt(float x, float y, float z);

	void setBackgroundColor(unsigned int r, unsigned int b, unsigned int g, unsigned int a);
	// adding and controling GUI elements
	void setFont(char *fontFile);
	bool doesGUIElementExists(unsigned int id); // check to see if any element exist
	IGUIElement *getElementFromId(unsigned int id);

	// GUI button
	unsigned int createButton(float startX, float startY, float endX, float endY);

	//GUI edit box
	unsigned int createEditBox(float startX, float startY, float endX, float endY);
	void setEditBoxPasswordBox(unsigned int id);
	void setEditBoxTextAlignment(unsigned int id, unsigned int horizontal, unsigned int vertical);
	void setEditBoxAutoScroll(unsigned int id, bool scroll);
	void setEditBoxWordWrap(unsigned int id, bool wrap);
	void setEditBoxMultiLine(unsigned int id, bool mLine);

	// GUI scroll bar
	unsigned int createScrollBar(bool horizontal, float startX, float startY, float endX, float endY);
	void setScrollBarMax(unsigned int id, int value);
	void setScrollBarPosition(unsigned int id, int value);
	int getScrollBarMax(unsigned int id);
	int getScrollBarPosition(unsigned int id);

	// GUI static text
	unsigned int createStaticText(float startX, float startY, float endX, float endY);
	void setStaticTextBackgroundColor(unsigned int id, unsigned int r, unsigned int g, unsigned int b, unsigned int a);

	// GUI message box. When created, all other GUIs will be disable till the this message box is closed
	unsigned int createMessageBox(wchar_t *caption, wchar_t *body, int flags);

	// GUI image
	unsigned int createImage(char *fileName, float x, float y);
	void setImageColor(unsigned int id, unsigned int r, unsigned int g, unsigned int b, unsigned int a);

	// GUI fade in - fade out effect
	unsigned int createInOutFader(float startX, float startY, float endX, float endY);
	void setInOutFaderFadeIn(unsigned int id, unsigned int value);
	void setInOutFaderFadeOut(unsigned int id, unsigned int value);
	bool InOutFaderIsReady(unsigned int id); // has it finished fading in/out
	void setInOutFaderColor(unsigned int id, unsigned int r, unsigned int g, unsigned int b, unsigned int a);

	// GUI SpinBox
	unsigned int createSpinBox(float startX, float startY, float endX, float endY);
	float getSpinBoxValue(unsigned int id);
	void setSpinBoxRange(unsigned int id, float min, float max, float step);
	void setSpinBoxDecimalPlaces(unsigned int id, unsigned int dp);

	// commone GUI controls
	void setGUIPosition(unsigned int id, float x, float y, float width, float height);
	//void setGUIMaxSize(unsigned int id, float width, float height);
	//void setGUIMinSize(unsigned int id, float width, float height);
	//void setGUISize(unsigned int id, float width, float height); // just use this. it make the max and min size the same
	void setGUIText(unsigned int id, const wchar_t *text);
	void setGUIText(unsigned int id, char* text);
	wchar_t* getGUIText(unsigned int id);
	void setGUIChild(unsigned int id, unsigned int childID); // so when the parent move, the child moves
	void setGUIFocus(unsigned int id);
	void removeGUIFocus(unsigned int id);
	bool hasGUIFocus(unsigned int id);

	void removeGUIElement(unsigned int id);

	// test, to send our own GUI events
	void sendGUIEvent(unsigned int callerID, unsigned int elementID, unsigned int eventID);

	//void startDraw(void);
	//void endDraw(void);

	void tick(void);
	void drawAll(void);

	// clear everythis so we can start all over
	void clearGUI(void);
	void clearSceneManager(void);

	// for Nodes
	bool doesNodeExists(unsigned int id); // check to see if any node exist
	ISceneNode *getNodeFromId(unsigned int id);

	// for testing, remove later
	unsigned int createTestBox(unsigned int r, unsigned int g, unsigned int b);

	// Animated mesh scene nodes
	unsigned int createAnimatedMesh(char *meshFileName);

	void setAnimationFrameLoop(unsigned int id, unsigned int start, unsigned int end);
	void setAnimationCurrentFrame(unsigned int id, unsigned int frameCount);
	unsigned int getAnimationCurrentFrame(unsigned int id);
	void setAnimationFrameSpeed(unsigned int id, float speed);
	void setAnimationLoop(unsigned int id, bool loop);
	void setAnimationTransitionTime(unsigned int id, float time);

	// 3D text
	unsigned int create3DText(char *fontName, char *text);

	// Particle system scene node
	unsigned int createParticle(void);
	// create emitters
	void createParticlePointEmitter(unsigned int id, float dirX, float dirY, float dirZ, unsigned int minParticlesPerSecond, unsigned int maxParticlesPerSecond, unsigned int minR, unsigned int minG, unsigned int minB, unsigned int minA, unsigned int maxR, unsigned int maxG, unsigned int maxB, unsigned int maxA, unsigned int lifeTimeMin, unsigned int lifeTimeMax, int maxAngleDegrees, float minX, float minY, float maxX, float maxY);
	// create effectors
	void addParticleFadeOutAffector(unsigned int id, unsigned int r, unsigned int g, unsigned int b, unsigned int a, unsigned int timeNeededToFadeOut);
	void addParticleGravityAffector(unsigned int id, float gravityX, float gravityY, float gravityZ, unsigned int timeForceLost);

	// Billboard
	unsigned int createBillboard(void);

	void setBillboardSize(unsigned int id, float x, float y);
	void setBillBoardColor(unsigned int id, unsigned int topR, unsigned int topG, unsigned int topB, unsigned int topA, unsigned int bottomR, unsigned int bottomG, unsigned int bottomB, unsigned int bottomA);

	// commone 3D Node controls
	void setNodePosition(unsigned int id, float x, float y, float z);
	void setNodeRotation(unsigned int id, float x, float y, float z);
	void setNodeScale(unsigned int id, float x, float y, float z);
	void setNodeParent(unsigned int id, unsigned int childID);
	void removeNode(unsigned int id);
	float getNodeX(unsigned int id);
	float getNodeY(unsigned int id);
	float getNodeZ(unsigned int id);

	// commone 3D SecenNode controls
	void setNodeTexture(unsigned int id, unsigned int textureLayer, char *fileName);
	void setNodeMaterialType(unsigned int id, unsigned int type);
	void setNodeMaterialFlag(unsigned int id, unsigned int flag, bool isOn);

	//////
	// Some vector functions
	//////
	void normalizeVector(float &x, float &y, float &z); // fix the vector so it has the length of 1
	void crossProductVector(float &forwardX, float &forwardY, float &forwardZ, float upX, float upY, float upZ); // get the cross produce, we use this with the up vector se can find the vector point to the side directin
	void directionToRotation(float &x, float &y, float &z); // turns a vector to angles in degree
	void rotationToDirection(float &x, float &y, float &z); // turn angles to vector direciton
	float vectorLength(float x, float y, float z);

	//MeshandGraphic functions
	unsigned int loadmap(irr::io::path modelname, unsigned int id);
	void CreateDefaultSkyBox();
	void AddCollision();
	unsigned int MetaSelector();
	void AddAnimatorToMesh(unsigned int MetaID, unsigned int MeshID, unsigned int animatr);
	unsigned int NodeAnimator();
	//GUI/HUD
	unsigned int DrawInventory(unsigned int id);
	unsigned int CreateImagegui(unsigned int InventoryID);
	unsigned int AddItemToSlot(unsigned int ItemPictureID, unsigned int imguiid);
	unsigned int CreateTexture(char* filename);
	unsigned int SetTexture(unsigned int id);
	void InvVisible(bool truefalse, unsigned int invid);

	void Wait(int seconds);

private:

	unsigned int ID; // we use this to make IDs for graphic objects
	std::map<unsigned int, ISceneNode*> id3DList; // where we keep our 3D graphic objects
	std::map<unsigned int, IGUIElement*> id2DList; // where we keep our 2D graphic objects
	std::map<unsigned int, IMetaTriangleSelector*> idTSList; //where we keep the selectors
	std::map<unsigned int, ISceneNodeAnimator*> idAList; //Animators collision detection and response
	std::map<unsigned int, ITexture*> idTexList; //Needed

	IrrlichtDevice *device;
	IGUIEnvironment *env;
	IVideoDriver *driver;
	ISceneManager *smgr;

	unsigned int oldTime;
	int oldMouseX, oldMouseY;

	ICameraSceneNode *camera;

	unsigned int bgR; // the background color when we start to draw
	unsigned int bgB;
	unsigned int bgG;
	unsigned int bgA;
};
