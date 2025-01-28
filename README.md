# **PintPath App**

The PintPath App is a cross-platform application built with Qt Quick and QML, designed to fetch and display brewery data from Ireland. The app integrates a RESTful API to provide real-time information and demonstrates key Qt functionalities, including the signal-slot mechanism, dynamic UI updates, and multi-platform deployment.

Key Features:
- Identify the **northernmost** and **southernmost** breweries.
- Display the brewery with the **longest name**.
- An interactive map with brewery filters.
- Supports both desktop and Android platforms.

---

## **Background Information**
This project was an opportunity to build my first Android application, with a focus on creating a cross-platform app. I started with minimal experience in Android app design and development, which made this project a fun adventure in build systems and app packaging.

I approached this project with the goal of not only delivering a basic and functional app but also exploring new skills and discovering the complexities of app development for Android, desktop, and physical hardware.

The project has:
- Deepened my familiarity with the Qt framework, including Qt Quick, QML, and C++/QML integration.
- Developed my understanding of UI design and functionality.
- Allowed me to tackle challenges in multi-platform development, such as API integration, data handling, and Android deployment.
- Built practical experience with tools like Qt Creator, Android permissions, and CMake.

---

## **Creation Process**

1. **Setup and Familiarization:**
   - Followed Qt Quick and Qt Creator tutorials from Qt Academy to understand core functionalities.
   - Learned how to set up emulators and configure CMake for different build systems.
   - Successfully deployed a basic app on Android to ensure development environment readiness.
   - Tested on different architectures to resolve platform-specific issues such as Android permissions and correct APK packaging.

2. **API Exploration:**
   - Read the Open Brewery API documentation and tested basic calls.
   - Implemented a server for using the API calls, then discovered that Qt has `QNetworkAccessManager` and replaced the server with a single class.
   - Learned how to connect C++ and QML code by registering C++ code with the engine.
   - Experimented with making API calls in the C++ backend of a Qt application.

3. **Backend Development:**
   - Implemented C++ classes to fetch and process brewery data, including determining:
     - Northernmost brewery.
     - Southernmost brewery.
     - Brewery with the longest name.
     - Breweries that also sell food.
   - Originally used a `std::vector` of structs but transitioned to a vendor model class after learning about Qt Models, which simplified data handling between C++ and QML.
   - Learned the difference between a creatable type and a non-creatable type.

4. **Frontend Development in QML:**
   - Designed a UI in Qt Quick for dynamically displaying brewery data.
   - Discovered how to correctly set up and manage IDE IntelliSense to ensure warnings were accurate and resolved.
   - Connected the C++ backend with the QML frontend for live data updates.
   - Added potential features like map integration, brewery filters (e.g., serving food), and different locator pins.
   - Used Qt Design elements to help create a responsive design that works on multiple devices.

5. **Testing and Deployment:**
   - Tested on desktop, Android emulator, and physical hardware "" to resolve platform-specific issues such as Android permissions and correct APK packaging.

---

## **Learning Experience**

# **What I Learned**
1. **Qt Framework:**
   - How to use Qt Creator and Qt Design together.
   - How to develop an app with Qt Quick and the different tools available in Qt's libraries.
   - How app deployment works and how to use AVDs to test multiple architectures.
   - Learned about Qt components like Qt Models, `QNetworkAccessManager`, and `MapView`.
   
2. **Cross-Platform Development:**
   - Gained practical experience deploying apps on desktop, Android Emulator, and physical devices.

3. **UI Challenges:**
   - Improved QML and Qt Quick design skills through experimentation and using tutorials from Qt Academy and documentation.
   - Created a `MapView` with the plugin setup in QML.

---

# **Challenges and Resolutions**
- **Linking OpenSSL to Android:**
  - Resolved through adjustments in `androidManifest.xml` and CMake linking.
  - Learned how to fetch content on CMake and move it to the right spot for `androidManifest.xml` permissions.
- **Handling Permissions:**
  - Learned to configure `androidManifest.xml` and `CMake` to handle necessary permissions for API calls.
- **Frontend Design:**
  - Utilized resources and examples to build a functional and visually appealing UI.
- **Switching to Qt Models:**
  - Transitioned from using a `std::vector`-based data structure to Qt Models for better compatibility with QML, inspired by Qt tutorials.

---

# **What Worked Well**
- Smooth integration of the C++ backend to the QML frontend.
- Transition to Qt Models improved data handling.

# **Areas for Improvement**
- My frontend development design skills.
- Enhancing the UI for a more polished look.
- Simplifying deployment on Android by streamlining the setup process.
- Expanding functionality to include breweries from other countries.

---

### **Running the App**

## Platforms
1. **Desktop:**
   - Compiler: MinGW
2. **Android:**
   - Emulator: API Level 35, x86_64 architecture
   - Physical Devices:
     - Samsung (ARM64 architecture, Google Play support)

---
## Installing
1. Clone the repository:
   ```bash
   git clone <repository_url>
   ```
2. Open the project in Qt Creator.
3. Install required Qt modules, including `Qt Quick` and `Qt for Android`.
4. Deploy:
   - For desktop: Select the desktop kit and run the app.
   - For Android: Set up the Android environment and deploy to the emulator or hardware.
