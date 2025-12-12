# Philosophers

![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![School](https://img.shields.io/badge/School-42-black?style=for-the-badge&logo=42)
![Threading](https://img.shields.io/badge/Threading-Pthread-orange?style=for-the-badge)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

<br />
<div align="center">
  <h3 align="center">I eat, therefore I sleep.</h3>
  <p align="center">
    42 Cursus Project - スレッドとミューテックスによる並行処理の学習
    <br />
    <a href="#about-the-project"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="#usage">View Usage</a>
    ·
    <a href="#issues">Report Bug</a>
  </p>
</div>

<details>
  <summary>Table of Contents</summary>
  <ol>
    <li><a href="#about-the-project">About The Project</a></li>
    <li><a href="#features">Features</a></li>
    <li><a href="#architecture">Architecture & Logic</a></li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#project-structure">Project Structure</a></li>
    <li><a href="#authors">Authors</a></li>
  </ol>
</details>

## About The Project

**Philosophers** は、古典的な計算機科学の問題である「食事する哲学者の問題 (Dining Philosophers Problem)」を解決するシミュレーションプログラムです。
Unixのスレッド (`pthread`) とミューテックス (`mutex`) を使用し、リソース（フォーク）の競合、データレースの防止、デッドロックの回避、そして正確な時間の管理を実装しています。

<div align="center">
  <img src="https://dummyimage.com/600x200/000/fff&text=./philo+5+800+200+200" alt="Simulation Demo">
</div>

## Features

- [x] **マルチスレッド実装**: 各哲学者を独立したスレッドとして動作させ、並行処理を実現
- [x] **デッドロック回避**: 偶数番目と奇数番目の哲学者でフォークを取る順序を変えるアルゴリズムを採用
- [x] **データレース防止**: `safe_functions` ラッパーとMutexにより、共有リソースへのアクセスを厳密に管理
- [x] **正確な時間管理**: `gettimeofday` を用いたミリ秒単位の経過時間計測と、ズレを補正する `precise_usleep` の実装
- [x] **監視スレッド**: 哲学者の餓死や食事回数制限を監視する独立したMonitorスレッド

## Architecture & Logic

### 1. 哲学者のライフサイクル (Philosopher Routine)

各哲学者は以下のループを繰り返します。

```mermaid
stateDiagram-v2
    [*] --> Thinking
    Thinking --> TakeFork1: Is Hungry
    TakeFork1 --> TakeFork2: Locked First Fork
    TakeFork2 --> Eating: Locked Second Fork
    Eating --> Sleeping: Finished Meal (Release Forks)
    Sleeping --> Thinking: Woke up
    
    state "Eating (Time to eat)" as Eating
    state "Sleeping (Time to sleep)" as Sleeping
