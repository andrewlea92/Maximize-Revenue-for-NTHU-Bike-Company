# 🚲 Maximize Revenue for NTHU Bike Company

This project is a **bike rental simulation system** designed for the NTHU Bike Company. The main goal is to **maximize total rental revenue** by efficiently allocating bikes to users based on time, location, and pricing constraints.

## 💡 Features

- Sort user requests by start time and ID
- Prioritize bikes with higher rental price
- Apply **Floyd-Warshall algorithm** to compute shortest paths between stations
- Support bike cooldown time and usage limits
- Smart bike transfer: if no suitable bike is at the user's station, search nearby stations for the most profitable option
- Apply rental discounts after each use
- Dynamic memory management for station capacities

## 📁 Input Files

- `bike_info.txt`: Initial price and discount rules per bike type  
- `map.txt`: Station connections with distances  
- `bike.txt`: Current status of all bikes  
- `user.txt`: List of user rental requests

## 🧾 Output Files

- `user_result.txt`: Rental result for each user  
- `station_status.txt`: Final status of all bikes at each station  
- `transfer_log.txt`: Bike transfer records (including delivery time if needed)

## 📈 Objective

- **Maximize total revenue** from all successful rentals  
- **Minimize failed requests** by searching feasible alternatives  

## 🛠️ Language & Algorithm

- Language: **C++**
- Algorithms: **QuickSort**, **Floyd-Warshall**, **Greedy selection**

---

> Designed as an advanced data structure and algorithmic optimization project for the Department of Computer Science, NTHU.
