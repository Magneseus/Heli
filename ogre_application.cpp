#include "ogre_application.h"
#include "bin/path_config.h"

namespace ogre_application
{
/* Materials */
const Ogre::String material_directory_g = MATERIAL_DIRECTORY;

OgreApplication::OgreApplication(void)
{
    /* Don't do work in the constructor, leave it for the Init() function */
}


void OgreApplication::Init(void)
{
	/* Set default values for the variables */
	input_manager_ = NULL;
	keyboard_ = NULL;
	mouse_ = NULL;

	/* Run all initialization steps */
    InitRootNode();
    InitPlugins();
    InitRenderSystem();
    InitWindow();
    InitViewport();
	InitEvents();
	InitOIS();

	// Re-enable debug window logging
	lm->getDefaultLog()->setDebugOutputEnabled(true);

	LoadMaterials();

	// Run our own initialization steps
	timeMod = Ogre::Real(1.0);

	// Set up the Sun light
	SunLight = ogre_scene_manager_->createLight("Sun");
	SunLight->setType(Ogre::Light::LT_DIRECTIONAL);
	SunLight->setDiffuseColour(Ogre::Real(0.985), Ogre::Real(1.0), Ogre::Real(0.75));
	SunLight->setSpecularColour(Ogre::Real(0.985), Ogre::Real(1.0), Ogre::Real(0.75));
	SunLight->setPowerScale(Ogre::Real(0.33));
	SunLight->setDirection(0, -1, 1);

	// Set up the sky box
	ogre_scene_manager_->setSkyBox(true, "MySky", 500);

	// Spawn the terrain
	Ogre::SceneNode* tmpGround = ogre_scene_manager_->getRootSceneNode()->createChildSceneNode();
	Ogre::Entity* tmpEnt = ogre_scene_manager_->createEntity("Desert_Ground.mesh");
	tmpEnt->setMaterialName("Material.001");
	tmpGround->attachObject(tmpEnt);
	tmpGround->setScale(200, 25, 200);
	tmpGround->setPosition(0, -5, 0);


	// Camera settings
	curCameraMode = CameraMode::FirstPerson;
	camZoomAmount = Ogre::Real(30.0);
	cameraModeSwitched = false;
	tabIsDown = false;

	camYaw = Ogre::Quaternion::IDENTITY;
	camPitch = Ogre::Quaternion::IDENTITY;


	// Spawn the player
	Ogre::SceneNode* tmp = ogre_scene_manager_->getRootSceneNode()->createChildSceneNode();
	PlayerEntity = new Player(ogre_scene_manager_, tmp);
	GameEntityList.push_back(PlayerEntity);

	if (curCameraMode == CameraMode::FirstPerson)
		BindCamera(PlayerEntity->getFPCameraNode());
	else
		BindCamera(PlayerEntity->getTPCameraNode());


	// TMP Spawn enemy
	tmp = ogre_scene_manager_->getRootSceneNode()->createChildSceneNode();
	Tank* tmpEnemy = new Tank(ogre_scene_manager_, tmp, PlayerEntity);
	GameEntityList.push_back(tmpEnemy);
	tmp->setPosition(-10, -5, 0);

	tmp = ogre_scene_manager_->getRootSceneNode()->createChildSceneNode();
	Jeep* tmpEnemy2 = new Jeep(ogre_scene_manager_, tmp, PlayerEntity);
	GameEntityList.push_back(tmpEnemy2);
	tmp->setPosition(10, -5, 0);

	tmp = ogre_scene_manager_->getRootSceneNode()->createChildSceneNode();
	AATower* tmpEnemy3 = new AATower(ogre_scene_manager_, tmp, PlayerEntity);
	GameEntityList.push_back(tmpEnemy3);
	tmp->setPosition(0, -5, 10);


	// Initialize the Debug Drawing Lib
	new DebugDrawer(ogre_scene_manager_, 0.5f);
}

void OgreApplication::MainLoop(void)
{
try {
	/* Main loop to keep the application going */
	ogre_root_->clearEventTimes();

	

	while (!ogre_window_->isClosed())
	{
		ogre_window_->update(false);
		ogre_window_->swapBuffers();
		// Build the debug frame
		DebugDrawer::getSingleton().build();
		ogre_root_->renderOneFrame();

		Ogre::WindowEventUtilities::messagePump();

		// If we want to exit, destroy the window
		if (wantToExit)
		{
			ogre_window_->destroy();
		}
	}
}
catch (Ogre::Exception &e) {
	throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
}
catch (std::exception &e) {
	throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
}
}


bool OgreApplication::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
	/* This event is called after a frame is queued for rendering */
	/* Do stuff in this event since the GPU is rendering and the CPU is idle */

	// Clear the debug drawer
	DebugDrawer::getSingleton().clear();

	/*  GAME UPDATE SECTION BELOW  */

	// Get the delta time
	Ogre::Real deltaTime = fe.timeSinceLastFrame;
	Ogre::Real deltaTimeAltered = deltaTime * timeMod;

	// Update all game entities
	for (auto it = GameEntityList.begin(); it != GameEntityList.end(); ++it)
	{
		(*it)->update(deltaTimeAltered);
	}


	/*   INPUT SECTION BELOW  */

	/* Capture input */
	keyboard_->capture();
	mouse_->capture();


	/* Handle specific key events */
	if (keyboard_->isKeyDown(OIS::KC_ESCAPE))
	{
		wantToExit = true;
		return false;
	}

	// Time dilation settings
	if (keyboard_->isKeyDown(OIS::KC_LSHIFT))
	{
		int scrollAmnt = mouse_->getMouseState().Z.rel / 120;

		if (timeMod > Ogre::Real(2.0))
		{
			timeMod += scrollAmnt * Ogre::Real(0.5);
		}
		else
		{
			timeMod += scrollAmnt * Ogre::Real(0.1);
		}
	}


	// Helicopter Controls
	{
		// Pitch +
		if (keyboard_->isKeyDown(OIS::KC_W) && !keyboard_->isKeyDown(OIS::KC_S))
			PlayerEntity->PitchDir = -1;
		// Pitch -
		else if (keyboard_->isKeyDown(OIS::KC_S) && !keyboard_->isKeyDown(OIS::KC_W))
			PlayerEntity->PitchDir = 1;
		else
			PlayerEntity->PitchDir = 0;

		// Roll +
		if (keyboard_->isKeyDown(OIS::KC_A) && !keyboard_->isKeyDown(OIS::KC_D))
			PlayerEntity->RollDir = 1;
		// Roll -
		else if (keyboard_->isKeyDown(OIS::KC_D) && !keyboard_->isKeyDown(OIS::KC_A))
			PlayerEntity->RollDir = -1;
		else
			PlayerEntity->RollDir = 0;

		// Yaw +
		if (keyboard_->isKeyDown(OIS::KC_Q) && !keyboard_->isKeyDown(OIS::KC_E))
			PlayerEntity->YawDir = 1;
		// Yaw -
		else if (keyboard_->isKeyDown(OIS::KC_E) && !keyboard_->isKeyDown(OIS::KC_Q))
			PlayerEntity->YawDir = -1;
		else
			PlayerEntity->YawDir = 0;

		// Lift +
		if (keyboard_->isKeyDown(OIS::KC_SPACE) && !keyboard_->isKeyDown(OIS::KC_C))
			PlayerEntity->LiftDir = 1;
		// Lift -
		else if (keyboard_->isKeyDown(OIS::KC_C) && !keyboard_->isKeyDown(OIS::KC_SPACE))
			PlayerEntity->LiftDir = -1;
		else
			PlayerEntity->LiftDir = 0;
	}

	// Mouse (Camera) Controls
	{
		// Camera switching (between first and third person)
		if (!tabIsDown && keyboard_->isKeyDown(OIS::KC_TAB))
		{
			tabIsDown = true;
			if (curCameraMode == CameraMode::FirstPerson)
			{
				curCameraMode = CameraMode::ThirdPerson;
				BindCamera(PlayerEntity->getTPCameraNode());
			}
			else
			{
				curCameraMode = CameraMode::FirstPerson;
				BindCamera(PlayerEntity->getFPCameraNode());
			}
		}
		else if (tabIsDown && !keyboard_->isKeyDown(OIS::KC_TAB))
		{
			tabIsDown = false;
		}

		// Camera mouse movement
		int mdeltaX = mouse_->getMouseState().X.rel;
		int mdeltaY = mouse_->getMouseState().Y.rel;

		float divi = 500.f;

		float xAng = -mdeltaX / divi;
		float yAng = -mdeltaY / divi;

		Ogre::Real curPitch = cameraSceneNode->getOrientation().getPitch().valueAngleUnits();
		bool shouldPitch = ((curPitch > Ogre::Real(-90.0) && yAng < 0.0f) ||
							(curPitch < Ogre::Real(90.0) && yAng > 0.0f));
		
		// First person, we can just pitch & yaw the camera node around
		if (curCameraMode == CameraMode::FirstPerson)
		{
			cameraSceneNode->yaw(Ogre::Radian(xAng), Ogre::Node::TS_PARENT);
			
			if (shouldPitch)
				cameraSceneNode->pitch(Ogre::Radian(yAng), Ogre::Node::TS_LOCAL);
		}
		// Third person we manually create and apply the quaternions for orbiting
		else if (curCameraMode == CameraMode::ThirdPerson)
		{
			Ogre::Vector3 transAmount = Ogre::Vector3(0.0, 0.0, camZoomAmount);

			// Get the quaternions
			Ogre::Quaternion extraYaw;
			extraYaw.FromAngleAxis(Ogre::Radian(xAng), Ogre::Vector3::UNIT_Y);
			camYaw = camYaw * extraYaw;

			// reset the position
			cameraSceneNode->setPosition(Ogre::Vector3(0.0, 0.0, 0.0));
			
			// Yaw
			cameraSceneNode->yaw(Ogre::Radian(xAng), Ogre::Node::TS_WORLD);

			// If we can still pitch, pitch
			if (shouldPitch)
			{
				Ogre::Quaternion extraPitch;
				extraPitch.FromAngleAxis(Ogre::Radian(yAng), Ogre::Vector3::UNIT_X);
				camPitch = camPitch * extraPitch;

				cameraSceneNode->pitch(Ogre::Radian(yAng));// , Ogre::Node::TS_WORLD);
			}

			cameraSceneNode->translate(camYaw * camPitch * transAmount, Ogre::Node::TS_WORLD);
		}

		// Camera zooming (only in third person
		if (curCameraMode == CameraMode::ThirdPerson &&
			camZoomAmount >= 10 && camZoomAmount <= 200 &&
			!keyboard_->isKeyDown(OIS::KC_LSHIFT))
		{
			camZoomAmount -= Ogre::Real(mouse_->getMouseState().Z.rel / 60);
			camZoomAmount = Ogre::Math::Clamp(
				camZoomAmount, 
				Ogre::Real(10), 
				Ogre::Real(200));
		}
	}

	return true;
}


Ogre::Camera* OgreApplication::BindCamera(Ogre::SceneNode* nodeToBind)
{
	// Get the camera, detach it from the scene node
	Ogre::Camera* camEnt = dynamic_cast<Ogre::Camera*>(cameraSceneNode->getAttachedObject("MyCamera"));

	if (camEnt != NULL)
	{
		cameraSceneNode->detachObject("MyCamera");

		// Attach the camera to the new scene node and set it as the new camera node
		nodeToBind->attachObject(camEnt);
		cameraSceneNode = nodeToBind;

		// Reset the camera's position
		camEnt->setPosition(0, 0, 0);

		// Return the camera entity
		return camEnt;
	}
	else
	{
		Ogre::String e("Camera object was unable to be detached from SceneNode : ");
		e.append(cameraSceneNode->getName());
		Error(e);
	}

	return NULL;
}


void OgreApplication::InitRootNode(void)
{
    try {
		// Initialize log manager
		lm = new Ogre::LogManager();
		lm->createLog(log_filename_g, true, false, false);

		/* We need to have an Ogre root to be able to access all Ogre functions */
        ogre_root_ = std::auto_ptr<Ogre::Root>(new Ogre::Root(config_filename_g, plugins_filename_g, ""));
		//ogre_root_->showConfigDialog();
    }
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
		throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitPlugins(void)
{
    try {

		/* Load plugin responsible for OpenGL render system */
        Strings plugin_names;
        plugin_names.push_back("RenderSystem_GL");
		
        Strings::iterator iter = plugin_names.begin();
        Strings::iterator iter_end = plugin_names.end();
        for (; iter != iter_end; iter++){
            Ogre::String& plugin_name = (*iter);
            if (OGRE_DEBUG_MODE){
                plugin_name.append("_d");
            }
            ogre_root_->loadPlugin(plugin_name);
        }

    }
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitRenderSystem(void)
{
    try {

        /* Load render system */
        const Ogre::RenderSystemList& render_system_list = ogre_root_->getAvailableRenderers();
        if (render_system_list.size() == 0)
        {
			throw(OgreAppException(std::string("OgreApp::Exception: Sorry, no rendersystem was found.")));
        }

        Ogre::RenderSystem *render_system = render_system_list.at(0);
        ogre_root_->setRenderSystem(render_system);

    }
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}

        
void OgreApplication::InitWindow(void)
{
    try {

        /* Create main window for the application */
		bool create_window_automatically = false;
        ogre_root_->initialise(create_window_automatically, window_title_g, custom_window_capacities_g);

        Ogre::NameValuePairList params;
        params["FSAA"] = "0";
        params["vsync"] = "true";
        ogre_window_ = ogre_root_->createRenderWindow(window_title_g, window_width_g, window_height_g, window_full_screen_g, &params);

        ogre_window_->setActive(true);
        ogre_window_->setAutoUpdated(false);
    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitViewport(void)
{
    try {

        /* Retrieve scene manager and root scene node */
        ogre_scene_manager_ = ogre_root_->createSceneManager(Ogre::ST_GENERIC, "MySceneManager");
        Ogre::SceneNode* root_scene_node = ogre_scene_manager_->getRootSceneNode();

        /* Create camera object */
        camera = ogre_scene_manager_->createCamera("MyCamera");
        cameraSceneNode = root_scene_node->createChildSceneNode("MyCameraNode");
		cameraSceneNode->attachObject(camera);

        camera->setNearClipDistance(camera_near_clip_distance_g);
        camera->setFarClipDistance(camera_far_clip_distance_g); 

		camera->setPosition(camera_position_g);
		camera->lookAt(camera_look_at_g);
		camera->setFixedYawAxis(true, camera_up_g);

        /* Create viewport */
        Ogre::Viewport *viewport = ogre_window_->addViewport(camera, viewport_z_order_g, viewport_left_g, viewport_top_g, viewport_width_g, viewport_height_g);

        viewport->setAutoUpdated(true);
        viewport->setBackgroundColour(viewport_background_color_g);

		/* Set aspect ratio */
		float ratio = float(viewport->getActualWidth()) / float(viewport->getActualHeight());
        camera->setAspectRatio(ratio);

    }
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitEvents(void)
{
	try {

		/* Add this object as a FrameListener for render events (see frameRenderingQueued event) */
		ogre_root_->addFrameListener(this);

		/* Add this object as a WindowEventListener to handle the window resize event */
		Ogre::WindowEventUtilities::addWindowEventListener(ogre_window_, this);

	}
    catch (Ogre::Exception &e){
		throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
		throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::InitOIS(void)
{
	/* Initialize the Object Oriented Input System (OIS) */
	try {

		/* Initialize input manager */
		OIS::ParamList pl; // Parameter list passed to the input manager initialization
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;
		ogre_window_->getCustomAttribute("WINDOW", &windowHnd);
		windowHndStr << windowHnd;
		pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
		pl.insert(std::make_pair(std::string("w32_mouse"), 
		std::string("DISCL_FOREGROUND" )));
		pl.insert(std::make_pair(std::string("w32_mouse"), 
		std::string("DISCL_NONEXCLUSIVE")));
		input_manager_ = OIS::InputManager::createInputSystem(pl);

		/*size_t hWnd = 0;
		ogre_window_->getCustomAttribute("WINDOW", &hWnd);
		input_manager_ = OIS::InputManager::createInputSystem(hWnd);*/

		/* Initialize keyboard and mouse */
		keyboard_ = static_cast<OIS::Keyboard*>(input_manager_->createInputObject(OIS::OISKeyboard, false));

		mouse_ = static_cast<OIS::Mouse*>(input_manager_->createInputObject(OIS::OISMouse, false));
		unsigned int width, height, depth;
		int top, left;
		ogre_window_->getMetrics(width, height, depth, left, top);
		const OIS::MouseState &ms = mouse_->getMouseState();
		ms.width = width;
		ms.height = height;

	}
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::LoadMaterials(void)
{
    try {
		
		/* Load materials that can then be assigned to objects in the scene */
		Ogre::String resource_group_name = "MyGame";
		Ogre::ResourceGroupManager& resource_group_manager = Ogre::ResourceGroupManager::getSingleton();
		resource_group_manager.createResourceGroup(resource_group_name);
		bool is_recursive = false;
		resource_group_manager.addResourceLocation(material_directory_g, "FileSystem", resource_group_name, is_recursive);
		resource_group_manager.initialiseResourceGroup(resource_group_name);
		resource_group_manager.loadResourceGroup(resource_group_name);

	}
    catch (Ogre::Exception &e){
        throw(OgreAppException(std::string("Ogre::Exception: ") + std::string(e.what())));
    }
    catch(std::exception &e){
        throw(OgreAppException(std::string("std::Exception: ") + std::string(e.what())));
    }
}


void OgreApplication::windowResized(Ogre::RenderWindow* rw)
{
	/* Update the window and aspect ratio when the window is resized */
	int width = rw->getWidth(); 
    int height = rw->getHeight();
      
    Ogre::SceneManager* scene_manager = ogre_root_->getSceneManager("MySceneManager");
    Ogre::Camera* camera = scene_manager->getCamera("MyCamera");

	if (camera != NULL){
		//std::cout << "1 " << (double)width/height << std::endl;
		camera->setAspectRatio(Ogre::Real((double)(width/height)));
    }

	const OIS::MouseState &ms = mouse_->getMouseState();
    ms.width = width;
    ms.height = height;

	ogre_window_->resize(width, height);
	ogre_window_->windowMovedOrResized();
	ogre_window_->update();
}


void OgreApplication::Error(Ogre::String errMessage, Ogre::String errPrefix)
{
	std::cout << errPrefix << errMessage << std::endl;
}


} // namespace ogre_application;
