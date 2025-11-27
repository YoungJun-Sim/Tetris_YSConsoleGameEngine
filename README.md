# 🎮 C++20 Console Tetris (Custom Engine)

> **A high-performance Tetris clone built from scratch using a custom game engine (`YSConsoleGameEngineLib`) on the Windows Console platform.**
> 
> *Developed using Modern C++ (C++20) and Visual Studio 2026.*

![Project Status](https://img.shields.io/badge/Status-Completed-success)
![Language](https://img.shields.io/badge/Language-C%2B%2B20-blue)
![Platform](https://img.shields.io/badge/Platform-Windows_Console-lightgrey)
![IDE](https://img.shields.io/badge/IDE-Visual_Studio_2026-purple)

<br/>
<p align="center">
  <a href="#-introduction">English</a> &nbsp;&nbsp;&nbsp;|&nbsp;&nbsp;&nbsp; <a href="#-프로젝트-소개-korean-ver">한국어 (Korean)</a>
</p>
<br/>

---

## 📖 Introduction
This project demonstrates a fully functional Tetris game running natively in the Windows Console environment. Unlike typical console applications, this project utilizes a **self-developed rendering engine** to achieve smooth 60 FPS animation without flickering. It strictly follows the **Modern Tetris Guideline**, implementing complex mechanics like SRS (Super Rotation System) and 7-Bag Randomizer.

---

## 📺 Gameplay Demo

| **Normal Play & Hold** |
|:---:|
| <img width="1362" height="792" alt="Tetris1" src="https://github.com/user-attachments/assets/55871fd0-5cbf-43f4-a8c2-6d55a689d04b" />
| *Smooth rendering & Ghost Piece* |

---

## 🛠️ Technical Highlights

### 1. Custom Console Engine (`YSConsoleGameEngineLib`)
Implemented a robust rendering pipeline using **Win32 API** to overcome the limitations of the standard console output.
* **Double Buffering:** Eliminated screen flickering by using `CreateConsoleScreenBuffer` and swapping buffers every frame.
* **Input Handling:** Wrapped `GetAsyncKeyState` to manage input latency (DAS/ARR) and prevent ghost inputs.
* **Performance:** Optimized for a stable 60 FPS execution loop using `clock()` based delta-time calculation.

### 2. Modern C++ Implementation (C++20)
Leveraged **C++20 features** to write concise, safe, and efficient code.
* **Smart Pointers:** Memory management using `std::unique_ptr` for game assets (Screens, Sprites, Fonts) to prevent memory leaks.
* **Standard Library:** Utilized `std::vector`, `std::algorithm` (shuffle), and `std::random_device` for logic implementation.

### 3. Super Rotation System (SRS)
Fully implemented the standard rotation rules used in modern Tetris games.
* **Wall Kicks:** When a piece rotates near a wall or floor, the game checks a pre-defined **Offset Table** to find a valid position, allowing for advanced moves like T-Spins.
* **Collision Detection:** Precise coordinate calculation to handle complex interactions between the active piece and the game field.

### 4. 7-Bag Randomizer
Implemented a fair randomizer system to ensure a balanced distribution of pieces.
* Uses `std::mt19937` (Mersenne Twister) to shuffle a "bag" of 7 unique tetrominoes.
* Prevents "droughts" (long periods without a specific piece, like the I-bar).

---

## 🎮 Game Features

* **Ghost Piece:** Displays where the current piece will land.
* **Hold System:** Allows the player to store and swap a piece for later use.
* **Leveling System:** Game speed increases dynamically as the score rises.
* **Hard Drop & Soft Drop:** Supports both instant dropping and accelerated lowering of pieces.
* **Score Calculation:** Rewards complex clears (Tetris, T-Spin, Perfect Clear) with higher scores.

---

## 💻 Development Environment

* **Language:** C++ (ISO C++20 Standard)
* **IDE:** Visual Studio 2026
* **OS:** Windows 10 / 11
* **Dependencies:** Windows.h (No 3rd-party game engines used)

<br/>
<br/>

---
---

## 🇰🇷 프로젝트 소개 (Korean Ver.)

> **직접 제작한 C++ 콘솔 게임 엔진(`YSConsoleGameEngineLib`)을 기반으로 구현한 고성능 테트리스입니다.**
> 
> *Modern C++ (C++20)과 Visual Studio 2026 환경에서 개발되었습니다.*

## 📖 개요
이 프로젝트는 상용 게임 엔진(Unity, Unreal) 없이, **Windows API와 C++만으로 렌더링 파이프라인과 게임 로직을 처음부터 구축**한 결과물입니다.
콘솔 환경의 한계인 화면 깜빡임을 자체 엔진으로 해결하여 부드러운 60프레임 애니메이션을 구현했으며, SRS(Super Rotation System)와 7-Bag Randomizer 등 모던 테트리스의 복잡한 시스템을 완벽하게 적용했습니다.

---

## 🛠️ 핵심 기술 (Technical Highlights)

### 1. 자체 제작 콘솔 엔진 (`YSConsoleGameEngineLib`)
기존 `system("cls")` 방식의 한계를 극복하기 위해 Win32 API를 활용한 전용 엔진을 구축했습니다.
* **더블 버퍼링 (Double Buffering):** `CreateConsoleScreenBuffer`를 활용하여 화면을 교차 출력함으로써 깜빡임(Flickering)을 완벽하게 제거했습니다.
* **입력 시스템:** `GetAsyncKeyState`를 래핑하여 키 입력의 지연 시간(DAS)과 반응 속도를 정밀하게 제어합니다.
* **리소스 관리:** 스프라이트, 폰트 등을 객체지향적으로 관리하여 유지보수성을 높였습니다.

### 2. Modern C++ (C++20) 적용
최신 C++ 표준인 **C++20**을 사용하여 안정적이고 효율적인 코드를 작성했습니다.
* **스마트 포인터:** `std::unique_ptr`를 사용하여 엔진 리소스의 메모리 누수를 방지하고 소유권을 명확히 했습니다.
* **표준 라이브러리:** `std::shuffle`, `std::mt19937` 등을 활용하여 신뢰성 있는 난수 생성과 로직을 구현했습니다.

### 3. SRS (Super Rotation System) 구현
현대 테트리스 가이드라인의 핵심인 회전 시스템을 구현했습니다.
* **월 킥 (Wall Kicks):** 블록 회전 시 벽이나 바닥에 막힐 경우, 미리 정의된 **오프셋 테이블(Offset Table)**을 순회하며 회전 가능한 위치를 찾아내는 알고리즘을 적용했습니다. (T-Spin 구현의 기반)
* **충돌 처리:** 블록과 맵 사이의 정밀한 좌표 계산을 통해 오차 없는 충돌 판정을 구현했습니다.

### 4. 7-Bag Randomizer
완전한 무작위가 아닌 공정한 게임을 위한 **7-Bag 시스템**을 적용했습니다.
* 7개의 블록을 한 세트로 묶어 섞는 방식을 사용하여, 특정 블록이 오랫동안 나오지 않는 현상을 방지하고 전략적인 플레이를 가능하게 했습니다.

---

## 🎮 주요 기능

* **고스트 블록 (Ghost Piece):** 블록이 떨어질 위치를 미리 계산하여 보여줍니다.
* **홀드 시스템 (Hold):** 현재 블록을 저장하고 필요할 때 꺼내 쓸 수 있습니다.
* **레벨링 시스템:** 점수가 오를수록 낙하 속도가 실시간으로 증가합니다.
* **특수 점수 계산:** T-Spin, Back-to-Back, 퍼펙트 클리어 등 고급 기술에 대한 가산점 로직이 포함되어 있습니다.

---

## 📝 개발자 노트

이 프로젝트는 단순한 게임 구현을 넘어, **Low-level 단계에서의 게임 엔진 아키텍처를 이해**하고 **메모리 관리 및 최적화** 능력을 기르기 위해 시작되었습니다. 특히 제약이 많은 콘솔 환경에서 부드러운 프레임 처리를 구현하는 과정에서 시스템 프로그래밍에 대한 깊은 이해를 얻을 수 있었습니다.

---

<p align="center">
  Developed by <b>[Simyj]</b>
</p>
