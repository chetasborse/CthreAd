



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
<!-- [![Contributors][contributors-shield]][contributors-url]
[![Forks][forks-shield]][forks-url]
[![Stargazers][stars-shield]][stars-url]
[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url] -->



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <h1 align="center">CthreAd</h1>
  <p align="center">
    <h3 align="center">A multi-threading library for C Programs</h3>
    <!-- <a href="https://github.com/othneildrew/Best-README-Template">View Demo</a> -->
    <p align="center">
    <a href="https://gitlab.com/andipro123/cthread/-/issues">Report Bug</a>
    ·
    <a href="https://gitlab.com/andipro123/cthread/-/issues">Request Feature</a>
    </p>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#one-one">One-One</a></li>
        <li><a href="#many-one">Many-One</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#testing">Testing</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
    <li><a href="#authors">Authors</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

CthreAd is a multi-threading library which implements POSIX compliant threads in the Linux userland. Primarily, we have implemented two models of multi-threading:

* **One-One**

    One-One model of multi-threading maps each user-level thread with a kernel-level thread. All the threads can access the kernel at the same time. Even if a thread makes a blocking system call (i/o, etc.), other threads can continue to run.

    <img src="https://www.tutorialspoint.com/assets/questions/media/12668/One%20to%20One%20multithreading%20model.PNG" alt="one-one">
    <br></br>

* **Many-One**

    The many to one model maps many of the user threads to a single kernel thread. This model is quite efficient as the user space manages the thread management. The library performs the context-switch and scheduling operations. 

    <img src="https://www.tutorialspoint.com/assets/questions/media/12668/Many%20to%20One%20multithreading%20model.PNG" alt="many-one">


<!-- GETTING STARTED -->
 
## Usage

To use CthreAd in your projects:

* Clone the repository using
    ```bash
    $ git clone https://gitlab.com/andipro123/cthread.git
    ```
* Move to the directory and choose the model you want to implement
    ```bash
    # for one-one
    $ cd cthread/one-one

    # for many-one
    $ cd cthread/many-one
    ```
* Compile the library
    ```bash
    $ gcc cthread.c -c -Wall

    #for many-one, use "cthread2.c"
    ```
* Include the header file in your project
    ```c
    // Your C program

    #include<cthread.h>
    .
    .
    .
    ```
* Compile your project using
    ```bash
    $ gcc <your-project.c> cthread.o - o <your-project>

    #for many-one, use "cthread2.o"
    ```

## Testing

The library comes with an automatic testing package to test all the functionalities of the threading library. Tests cover all areas of multi-threading to ensure that  basic functions are working as expected. To run the tests:

```bash 
$ make run
```
<!-- CONTRIBUTING -->
## Contributing

This library is not a production level multi-threading library yet. Please use caution while using it in your production softwares. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request


<!-- ACKNOWLEDGEMENTS -->
## References
* [TutorialsPoint](https://www.tutorialspoint.com/multi-threading-models#:~:text=Multithreading%20allows%20the%20execution%20of,of%20the%20CPU%20by%20multitasking.)
* [GeeksForGeeks](https://www.geeksforgeeks.org/)


## Authors

- [@chetasborse](https://github.com/chetasborse)
- [@andipro123](https://github.com/andipro123)

