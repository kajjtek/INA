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
    """
    Calculates the distance to the k_neighbors-th nearest neighbor for each point.
    Used for the Elbow Method to determine an optimal 'eps' for DBSCAN.

    Args:
        data (list of lists/NumPy arrays): The dataset.
        k_neighbors (int): The 'k' in k-nearest neighbors (usually min_samples for DBSCAN).

    Returns:
        list of float: Sorted distances to the k_neighbors-th nearest neighbor for all points.
    """
    if isinstance(data, np.ndarray):
        data_list = data.tolist()
    else:
        data_list = [list(point) for point in data]

    distances = []
    # This can be computationally expensive for large datasets
    for i in range(len(data_list)):
        point_distances = []
        for j in range(len(data_list)):
            if i != j: # Don't calculate distance to itself
                point_distances.append(euclidean_distance(data_list[i], data_list[j]))
        point_distances.sort()
        if len(point_distances) >= k_neighbors:
            distances.append(point_distances[k_neighbors - 1]) # Get the k_neighbors-th smallest distance
    return sorted(distances)

def plot_k_distances(k_distances, k_neighbors):
    """
    Plots the k-distance graph for DBSCAN epsilon selection.

    Args:
        k_distances (list of float): Sorted distances to the k-th nearest neighbor.
        k_neighbors (int): The 'k' used for distance calculation.
    """
    plt.figure(figsize=(10, 6))
    plt.plot(k_distances)
    plt.ylabel(f"Distance to {k_neighbors}-th nearest neighbor")
    plt.xlabel("Points sorted by distance")
    plt.title("K-distance graph for DBSCAN epsilon selection (Elbow Method)")
    plt.grid(True)
    plt.show()

def evaluate_dbscan_clusters(true_labels, dbscan_labels):
    """
    Evaluates DBSCAN clustering results by calculating noise percentage,
    classification accuracy (for points in clusters), and misclassification
    percentage within identified clusters.

    Args:
        true_labels (list/NumPy array): The actual digit labels (0-9).
        dbscan_labels (list of int): The cluster IDs assigned by DBSCAN.

    Returns:
        tuple: A tuple containing:
            - noise_percentage (float): Percentage of data points identified as noise.
            - accuracy (float): Classification accuracy for points within clusters.
            - misclassification_percentage_in_clusters (float): Percentage of misclassified
                                                                points within clusters.
    """
    num_points = len(true_labels)
    
    # Ensure labels are lists for consistency
    if isinstance(true_labels, np.ndarray):
        true_labels = true_labels.tolist()
    if isinstance(dbscan_labels, np.ndarray):
        dbscan_labels = dbscan_labels.tolist()

    # 1. Calculate noise percentage
    noise_count = sum(1 for label in dbscan_labels if label == -1)
    noise_percentage = (noise_count / num_points) * 100

    # Group points by cluster ID (excluding noise)
    cluster_to_true_labels = {}
    for i in range(num_points):
        cluster_id = dbscan_labels[i]
        if cluster_id != -1: # Exclude noise points
            if cluster_id not in cluster_to_true_labels:
                cluster_to_true_labels[cluster_id] = []
            cluster_to_true_labels[cluster_id].append(true_labels[i])
    
    total_correctly_classified = 0
    total_in_clusters = 0
    total_incorrectly_classified_in_clusters = 0

    # 2. Calculate accuracy and misclassification within clusters
    for cluster_id, true_l_list in cluster_to_true_labels.items():
        if not true_l_list: # Skip empty clusters (shouldn't happen with core point logic)
            continue

        total_in_clusters += len(true_l_list)
        
        # Find the dominant digit (true label) in this cluster
        most_common_digit = Counter(true_l_list).most_common(1)[0][0]
        
        # Count correct and incorrect classifications within this cluster
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

    # Combine train and test data for clustering
    all_images = np.concatenate((x_train, x_test), axis=0)
    all_labels = np.concatenate((y_train, y_test), axis=0)

    # Normalize and flatten
    all_images_normalized = all_images / 255.0
    data_points = all_images_normalized.reshape(-1, 784).tolist() # Convert to list of lists
    true_labels_list = all_labels.tolist()

    print("\n" + "="*30 + "\n--- Running DBSCAN Clustering Tasks ---")

    # Task 2.1: Implement DBSCAN
    # This is done by the dbscan_algorithm function above.

    # Task 2.2: Choose appropriate eps and min_samples
    # Suggestion: Use k-distance graph to help select eps
    # For MNIST, 784 dimensions can make distance calculations very large.
    # It's highly recommended to normalize data to 0-1 for DBSCAN, which was done above.
    # min_samples is often chosen as 2 * dimensionality_of_data, but for high dimensions
    # like 784, this would be too large. Start with smaller values like 5-50.
    
    # Calculate k-distances for epsilon selection (e.g., for min_samples=20)
    # This step can be very computationally intensive for large datasets (like MNIST)
    # If it takes too long, you might want to sample a subset of your data for this analysis.
    # print("\n--- Calculating k-distances for DBSCAN parameter tuning (This might take a while for large datasets) ---")
    # Choosing min_samples = 20 as an example. You should experiment with this value.
    # try:
    #     k_for_dbscan_plot = 20 # A common starting point for min_samples
    #     dbscan_k_distances = calculate_k_distances(data_points, k_for_dbscan_plot)
    #     plot_k_distances(dbscan_k_distances, k_for_dbscan_plot)
    #     print(f"K-distance graph plotted. Look for an 'elbow' to choose eps.")
    # except Exception as e:
    #     print(f"Could not calculate k-distances (might be too slow for full dataset or {e}). Skipping plot.")


    # Placeholder for chosen eps and min_samples after analysis of k-distance graph
    # !!! YOU WILL NEED TO EXPERIMENT WITH THESE VALUES BASED ON YOUR K-DISTANCE PLOT AND DATA !!!
    # These are illustrative values and likely need tuning for optimal results on EMNIST MNIST
    chosen_eps = 0.5 # Example value, you must select this based on your plot
    chosen_min_samples = 20 # Example value, you must select this based on your plot

    
    dbscan_assignments = dbscan_algorithm(data_points, eps=chosen_eps, min_samples=chosen_min_samples)

    # Task 2.3: Evaluate DBSCAN results
    evaluate_dbscan_clusters(true_labels_list, dbscan_assignments)



    for eps in range(8,16):
        for min_samples in range(10,20):
            print(f"\n--- Running DBSCAN with chosen parameters: eps={eps}, min_samples={min_samples} ---")
            dbscan_assignments, n_clasters = dbscan_algorithm(data_points, eps=eps, min_samples=min_samples)
            if 10 <= n_clasters <= 30:
                evaluate_dbscan_clusters(true_labels_list, dbscan_assignments)
            else:
                print(f"\n-----cluster number={n_clasters}\n")