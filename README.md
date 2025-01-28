

# **Brewery Explorer App**

## **Introduction**
The Brewery Explorer App is a cross-platform application built with Qt Quick and QML, designed to fetch and display brewery data from Ireland. The app integrates a RESTful API to provide real-time information and demonstrates key Qt functionalities, including the signal-slot mechanism, dynamic UI updates, and multi-platform deployment.

Key Features:
- Identify the **northernmost** and **southernmost** breweries.
- Display the brewery with the **longest name**.
- An interactive map with brewery filters.
- Supports both desktop and Android platforms.


## **Background Information**  //TODO: this needs work
  This project was an opportunity to build my first Android application, with a focus on creating a cross-platform app. I started with minimal experience in Android app design and development, which made this project a fun adventure in build systems and app packaging.
  I approached this project with the goal of not only delivering a basic and functional app but also exploring new skills and discovering the complexities of app development for Android, desktop, and physical hardware.
  The project has:
    - Deepened my familiarity with the Qt framework, including Qt Quick, QML and C++/QML intergration.
    - Developed my understaning of UI design and functionality.
    - Allowed me to tackle challenges in multi-platform development, such as API integration, data handling, and Android deployment.
    - Built practical experience with tools like Qt Creator, Android permissions, and CMake.

## **Creation Process**

1. **Setup and Familiarization:**
   - Followed Qt Quick and Qt Creator tutorials from Qt Academy to understand core functionalities.
   - Learned how to set up emulators and configure CMake for different build systems.
   - Successfully deployed a basic app on Android to ensure development environment readiness.
    - Tested on different architecture to resolve platform-specific issues such as Android permissions and correct APK packaging.
2. **API Exploration:**
   - Read the Open Brewery API documentation and tested basic calls.
   - Implimented a server for using the API calls, discovered that Qt has QNetworkAccessManager and replaced the server with one class.
   - Learned how to connect C++ and QML code through regestering C++ code with the engine 
   - Experimented with making API calls in the C++ backend of a Qt application.
3. **Backend Development:**
   - Implemented C++ classes to fetch and process brewery data, including determining:
     - Northernmost brewery.
     - Southernmost brewery.
     - Brewery with the longest name.
     - Breweries that also sell food.
    - This class originally was written with a std::vector of structs however once I started implimenting the MapView I learned about how to use models in QML and implimented a vendor model class to simplify data handling between C++ and QML.
    - Learned the differnce between a createable type and a non-createable type
4. **Frontend Development in QML:**
   - Designed a UI in Qt Quick for displaying brewery data dynamically.
   - Discovered how to correctly set up and manage IDE intellisense to make sure warnings were correct and fixed if they were accurate.
   - Connected the C++ backend with the QML frontend using for live data updates.
   - Added potential features like map integration, brewery filters (e.g., serving food), and different locator pins.
   - Used Qtdesign elements to help create responsive design that works on multiple devices.
5. **Testing and Deployment:**
   - Tested on desktop, Android emulator, and physical hardware, iterating to resolve platform-specific issues such as Android permissions and correct APK packaging.


## **Learning Experience**

### **What I Learned**
1. **Qt Framework:**
   - How to use Qtcreator and Qtdesign together.
   - How to develop an app with Qtquick and the different tools available in Qt's libraries.
   - How app deployment works and how to use AVDs to test multiple architectures.
   - Learned about Qt components like Qt Models, QNetworkAccessManager and MapView.
3. **Cross-Platform Development:**
   - Gained practical experience deploying apps on desktop, Android Emulator, and physical devices.
4. **UI Challenges:**
   - Improved QML and Qt Quick design skills through experimentation and using tutorials from Qt Academy and documentation.
   - Creating a MapView with the Plugin setup in QML

### **Challenges and Resolutions**
- **Linking OpenSSL to Android:**
  - Resolved through research and adjustments in `androidManifest.xml` and CMake linking.
  - How to fetch content on CMake and move to the right spot for the androindManifest.xml permissions
- **Handling Permissions:**
  - Learned to configure `androidManifest.xml` and `CMake` to handle necessary permissions for API calls.
- **Frontend Design Struggles:**
  - Utilized resources and examples to build a functional and visually appealing UI.
- **Switching to Qt Models:**
  - Transitioned from using a `std::vector`-based data structure to Qt Models for better compatibility with QML, inspired by Qt tutorials.

## **My Reflection**

### What Worked Well
- Smooth integration of C++ backend to QML frontend.
- Transition to Qt Models improved data handling.

### Areas for Improvement
- My patients with frontend development.
- Enhancing the UI for a more polished look.
- Simplifying deployment on Android by streamlining the setup process.
- Expand functionality to include breweries from other countries.


## **Testing**

### Platforms
1. **Desktop:**
   - Compiler: MinGW
2. **Android:**
   - Emulator: API Level 35, x86_64 architecture
   - Physical Devices:
     - Samsung (ARM64 architecture, Google Play support)


## **Information**

### **Running the App**
1. Clone the repository:
   ```bash
   git clone <repository_url>
   ```
2. Open the project in Qt Creator.
3. Install required Qt modules, including `Qt Quick` and `Qt for Android`.
4. Deploy:
   - For desktop: Select the desktop kit and run the app.
   - For Android: Set up the Android environment and deploy to the emulator or hardware.

