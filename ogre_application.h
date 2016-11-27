#ifndef OGRE_APPLICATION_H_
#define OGRE_APPLICATION_H_

#include <exception>
#include <string>

#include "OGRE/OgreRoot.h"
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OIS/OIS.h"

#include "Shapes.h"
#include "GameEntity.h"
#include "Player.h"
#include "Enemy.h"
#include "Tank.h"
#include "Jeep.h"
#include "AATower.h"

namespace ogre_application {
	/* A useful type to define */
	typedef std::vector<Ogre::String> Strings;

	/* Our exception type */
	class OgreAppException: public std::exception
    {
	private:
		std::string message_;
	public:
		OgreAppException(std::string message) : message_(message) {};
		virtual const char* what() const throw() { return message_.c_str(); };
	};

	/* Our Ogre application */
	class OgreApplication :
	public Ogre::FrameListener, // Derive from FrameListener to be able to have render event callbacks
    public Ogre::WindowEventListener // Derive from FrameListener to be able to have window event callbacks
	{

    public:
        OgreApplication(void);
		// Call Init() before running the main loop
        void Init(void); 
		// Keep application active
        void MainLoop(void); 

		// Bind Camera to a given scene node
		Ogre::Camera* BindCamera(Ogre::SceneNode* nodeToBind);

		// Sun light
		Ogre::Light* SunLight;

		//////////// Public Variables ///////////////

		// Create root that allows us to access Ogre commands
		std::auto_ptr<Ogre::Root> ogre_root_;
		// Main Ogre window
		Ogre::RenderWindow* ogre_window_;

		// Input stuffs
		OIS::Mouse *mouse_;
		OIS::Keyboard *keyboard_;

    private:
		// Create our own logger to remove the console spam
		Ogre::LogManager* lm;
		// Input managers
		OIS::InputManager *input_manager_;
		// Ogre Scene Manager
		Ogre::SceneManager* ogre_scene_manager_;
		// bool to toggle exiting the game
		bool wantToExit = false;

		// Time
		Ogre::Real timeMod;

		// Camera Scene Node
		Ogre::SceneNode* cameraSceneNode;
		// Camera object
		Ogre::Camera* camera;
		// FP / TP camera mode
		enum CameraMode {FirstPerson, ThirdPerson, COUNT};
		CameraMode curCameraMode;
		// Third person zoom amount
		Ogre::Real camZoomAmount;
		Ogre::Quaternion camYaw;
		Ogre::Quaternion camPitch;

		// Input controls for camera mode switching
		bool tabIsDown;
		bool cameraModeSwitched;

		// Player Entity
		Player* PlayerEntity;
		// List of in game entities
		std::vector<GameEntity*> GameEntityList;

		/* Methods to initialize the application */
		void InitRootNode(void);
		void InitPlugins(void);
		void InitRenderSystem(void);
		void InitWindow(void);
		void InitViewport(void);
		void InitEvents(void);
		void InitOIS(void);
		void LoadMaterials(void);

		/* Methods to handle events */
		bool frameRenderingQueued(const Ogre::FrameEvent& fe);
		void windowResized(Ogre::RenderWindow* rw);

		// Error handling message
		void Error(Ogre::String, Ogre::String = Ogre::String(""));

	private:
		/* Some configuration constants */
		/* They are written here as global variables, but ideally they should be loaded from a configuration file */

		/* Initialization */
		const Ogre::String config_filename_g = "";
		const Ogre::String plugins_filename_g = "";
		const Ogre::String log_filename_g = "Ogre.log";

		/* Main window settings */
		const Ogre::String window_title_g = "Demo";
		const Ogre::String custom_window_capacities_g = "";
		const unsigned int window_width_g = 1600;
		const unsigned int window_height_g = 900;
		const bool window_full_screen_g = false;

		/* Viewport and camera settings */
		float viewport_width_g = 0.95f;
		float viewport_height_g = 0.95f;
		float viewport_left_g = (1.0f - viewport_width_g) * 0.5f;
		float viewport_top_g = (1.0f - viewport_height_g) * 0.5f;
		unsigned short viewport_z_order_g = 100;
		const Ogre::ColourValue viewport_background_color_g = Ogre::ColourValue(0.0, 0.0, 0.0);
		float camera_near_clip_distance_g = 0.01f;
		float camera_far_clip_distance_g = 10000.0f;
		Ogre::Vector3 camera_position_g = Ogre::Vector3(0.5, 0.5, 5.5);
		Ogre::Vector3 camera_look_at_g = Ogre::Vector3(0.0, 0.0, 0.0);
		Ogre::Vector3 camera_up_g = Ogre::Vector3(0.0, 1.0, 0.0);

    }; // class OgreApplication

} // namespace ogre_application;

#endif // OGRE_APPLICATION_H_
