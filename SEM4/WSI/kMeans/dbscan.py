import random
import math
import numpy as np
from collections import Counter
import matplotlib.pyplot as plt
import seaborn as sns

def euclidean_distance(point1, point2):
    distance = 0;
    if(len(point1)!=len(point2)): raise ValueError("points must have the same dimension")

    for i in range(len(point1)):
        distance += (point1[i]-point2[i])**2
    return math.sqrt(distance)

def findNeighbours(point_idx, data):
    neighbours = []
    for i in range(len(data)):
        if euclidean_distance(data[i], data[point_idx]):
            neighbours.append(i)
    return neighbours

def dbscan_algorithm(dataRaw, eps, min_samples):
    if isinstance(dataRaw, np.ndarray):
        data = dataRaw.tolist()
    else:
        data = [list(point) for point in dataRaw]

    labels = [0] * len(data)
    cluster_id = 0

    print(f"Starting DBSCAN with eps={eps}, min_samples={min_samples}...")

    for i in range(len(data)):
        if(labels[i]!=0): continue

        neighbours = findNeighbours(i, data)
        if len(neighbours)<min_samples:
            labels[i]=-1
            continue
            
        cluster_id+=1
        labels[i]=cluster_id

        queue = list(neighbours)
        while queue:
            current_id = queue.pop(0)

            if labels[current_id] == -1:
                labels[current_id] = cluster_id
            
            if labels[current_id] != 0:
                continue

            labels[current_id] = cluster_id

            current_neighbours = findNeighbours(current_id, data)
            if(len(current_neighbours)>=min_samples):
                for neighbour_id in current_neighbours:
                    if labels[neighbour_id] == 0:
                        queue.append(neighbour_id)
    
    print(f"DBSCAN completed. Found {cluster_id} clusters and {labels.count(-1)} noise points.")

    return labels, cluster_id


def calculate_k_distances(data, k_neighbors):
    if isinstance(data, np.ndarray):
        data_list = data.tolist()
    else:
        data_list = [list(point) for point in data]

    distances = []
    for i in range(len(data_list)):
        point_distances = []
        for j in range(len(data_list)):
            if i != j: 
                point_distances.append(euclidean_distance(data_list[i], data_list[j]))
        point_distances.sort()
        if len(point_distances) >= k_neighbors:
            distances.append(point_distances[k_neighbors - 1]) # Get the k_neighbors-th smallest distance
    return sorted(distances)

def plot_k_distances(k_distances, k_neighbors):
    plt.figure(figsize=(10, 6))
    plt.plot(k_distances)
    plt.ylabel(f"Distance to {k_neighbors}-th nearest neighbor")
    plt.xlabel("Points sorted by distance")
    plt.title("K-distance graph for DBSCAN epsilon selection (Elbow Method)")
    plt.grid(True)
    plt.show()

def evaluate_dbscan_clusters(true_labels, dbscan_labels):
    num_points = len(true_labels)
    
    if isinstance(true_labels, np.ndarray):
        true_labels = true_labels.tolist()
    if isinstance(dbscan_labels, np.ndarray):
        dbscan_labels = dbscan_labels.tolist()

    noise_count = sum(1 for label in dbscan_labels if label == -1)
    noise_percentage = (noise_count / num_points) * 100

    cluster_to_true_labels = {}
    for i in range(num_points):
        cluster_id = dbscan_labels[i]
        if cluster_id != -1: 
            if cluster_id not in cluster_to_true_labels:
                cluster_to_true_labels[cluster_id] = []
            cluster_to_true_labels[cluster_id].append(true_labels[i])
    
    total_correctly_classified = 0
    total_in_clusters = 0
    total_incorrectly_classified_in_clusters = 0

    for cluster_id, true_l_list in cluster_to_true_labels.items():
        if not true_l_list: 
            continue

        total_in_clusters += len(true_l_list)
        
       
        most_common_digit = Counter(true_l_list).most_common(1)[0][0]
        
        
        correct_in_cluster = sum(1 for l in true_l_list if l == most_common_digit)
        incorrect_in_cluster = len(true_l_list) - correct_in_cluster
        
        total_correctly_classified += correct_in_cluster
        total_incorrectly_classified_in_clusters += incorrect_in_cluster
    
    accuracy = (total_correctly_classified / total_in_clusters) * 100 if total_in_clusters > 0 else 0
    misclassification_percentage_in_clusters = (total_incorrectly_classified_in_clusters / total_in_clusters) * 100 if total_in_clusters > 0 else 0

    print(f"\n--- DBSCAN Evaluation Results ---")
    print(f"Procent szumu (Noise Percentage): {noise_percentage:.2f}%")
    print(f"Dokładność klasyfikacji w klastrach (Accuracy in Clusters): {accuracy:.2f}%")
    print(f"Procent błędnych klasyfikacji w klastrach (Misclassification in Clusters): {misclassification_percentage_in_clusters:.2f}%")
    
    return noise_percentage, accuracy, misclassification_percentage_in_clusters

if __name__ == '__main__':
    import tensorflow as tf
    (x_train, y_train), (x_test, y_test) = tf.keras.datasets.mnist.load_data()

    
    all_images = np.concatenate((x_train, x_test), axis=0)
    all_labels = np.concatenate((y_train, y_test), axis=0)

    num_samples_to_select = 15000
    indices = np.random.choice(len(all_images), num_samples_to_select, replace=False)
    selected_images = all_images[indices]
    selected_labels = all_labels[indices]

    
    all_images_normalized = selected_images / 255.0
    data_points = all_images_normalized.reshape(-1, 784).tolist()
    true_labels_list = selected_labels.tolist()

    print("\n" + "="*30 + "\n--- Running DBSCAN Clustering Tasks ---")

    chosen_eps = 0.5 
    chosen_min_samples = 20 

    
    dbscan_assignments = dbscan_algorithm(data_points, eps=chosen_eps, min_samples=chosen_min_samples)

    
    evaluate_dbscan_clusters(true_labels_list, dbscan_assignments)



    for eps in range(8,16):
        for min_samples in range(10,20):
            print(f"\n--- Running DBSCAN with chosen parameters: eps={eps}, min_samples={min_samples} ---")
            dbscan_assignments, n_clasters = dbscan_algorithm(data_points, eps=eps, min_samples=min_samples)
            if 10 <= n_clasters <= 30:
                evaluate_dbscan_clusters(true_labels_list, dbscan_assignments)
            else:
                print(f"\n-----cluster number={n_clasters}\n")