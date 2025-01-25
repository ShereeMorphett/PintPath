## Initial approach

- [X] Setup and Familiarization
  Go through basic Qt Quick and Qt Creator tutorials, including deploying a simple app on Android. - 

- [ ] Understanding Open Brewery API and Data Handling
Explore API documentation and test API calls using tools like Postman or a browser.
Experiment with making API calls from C++ code in a Qt environment.
Backend Development (1-2 days)

- [ ] Implement C++ classes to fetch and process data from the API.
Identify northernmost, southernmost, and longest-name breweries.
Frontend Development in QML (2 days)

- [ ] Design the UI to display brewery information.
  Add eye candy, like a map, using Qt's location or map components.
  Link C++ backend with QML frontend using the signal-slot mechanism.
  Testing and Deployment (2-3 days)

- [ ] Run the app on the desktop, Android emulator, and physical hardware.
  Debug and fix issues that arise during deployment.
  Documentation and Finalization (1 day)

- [ ] Write the README.md, detailing the learning process, challenges, and the creation workflow.
    
- [X]  Prepare the GitHub repository.
      
- [ ]  Link the GitHubs.

## resources
https://doc.qt.io/qt-6/qnetworkaccessmanager.html

https://www.qt.io/blog/restful-client-applications-in-qt-6.7-and-forward

-https://www.openbrewerydb.org/documentation - Open Brewery Rest API's

-https://doc.qt.io/qt-6/android.html - Qt for Android documentation, where you can get started

-https://academy.qt.io/catalog - Learning courses where you can find help, for example "Introduction to Qt Quick"


## Issues I ran into
linking openSSL to android, learning androindManifest.xml  Cmake linking, how do fetch content on cmake and move to the right spot for the androindManifest.xml permissions
Qtdesign tab on Qtcreator, ended up not using it and doing it by hand

## Testing
- Desktop
   - MinGW
- Android (unknown version) API level 35
    - CPU architecture x86_64
    - Google Play
- Samsung
    - CPU architecture ARM64
    - Google Play
