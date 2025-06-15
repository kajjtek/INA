import random
import math
import numpy as np
from collections import Counter
import matplotlib.pyplot as plt
import seaborn as sns


#helping functions

def euclidean_distance(point1, point2):
    distance = 0;
    if(len(point1)!=len(point2)): raise ValueError("points must have the same dimension")

    for i in range(len(point1)):
        distance += (point1[i]-point2[i])**2
    return math.sqrt(distance)

def sum_vectors(vec1, vec2):
    if len(vec1) != len(vec2):
        raise ValueError("Vectors must have the same dimension for summation.")
    return [a+b for a,b in zip(vec1, vec2)]

def divide_vector(vector, divisor):
    if divisor==0:
        raise ValueError("cannot divide by zero")
    return [x/divisor for x in vector]

def initialise_centroid(data, k):
    centroids = []

    first_centroid = random.choice(data)
    centroids.append(first_centroid)

    for _ in range(1, k):
        distances = []
        for image in data:
            min_dist = float('inf')
            for c in centroids:
                dist = euclidean_distance(image, c)**2
                min_dist=min(min_dist, dist)
            distances.append(min_dist)

        total_distance = sum(distances)

        if(total_distance==0):
            new_centroid = random.choice(data)
        else:
            propabilities = [d/total_distance for d in distances]
            new_centroid = random.choices(data, weights=propabilities, k=1)[0]

        centroids.append(new_centroid)
    
    return centroids

#the algorithm
def kmeans_algorith(data, k, max_iter=300, tol=1e-4):

    centroids = initialise_centroid(data, k)

    current_centroids = [list(c) for c in centroids]

    for iteration in range(max_iter):
        assignments = [0] * len(data)

        for i, image in enumerate(data):
            min_dist = float('inf')
            closest_centroid_id = -1
            for j, c in enumerate(centroids):
                dist = euclidean_distance(image, c)
                if(dist<min_dist):
                    min_dist=dist
                    closest_centroid_id=j
            assignments[i] = closest_centroid_id

        new_centroids = [[0.0]*len(data[0]) for _ in range(k)]
        cluster_counts = [0]*k

        for i, image in enumerate(data):
            cluster_id = assignments[i]
            new_centroids[cluster_id] = sum_vectors(new_centroids[cluster_id], image)
            cluster_counts[cluster_id]+=1
        
        max_shift = 0.0
        for j in range(k):
            if cluster_counts[j]>0:
                new_centroids[j] = divide_vector(new_centroids[j], cluster_counts[j])
                shift = euclidean_distance(current_centroids[j], new_centroids[j])
                max_shift=max(shift, max_shift)
                current_centroids[j]=new_centroids[j]
            else:
                current_centroids[j] = random.choice(data)
        
        if max_shift<tol:
            break
    
    return centroids, assignments

def run_experiment(k, repetitions, dataRaw):
    
    if isinstance(dataRaw, np.ndarray):
        data=dataRaw.tolist()
    else:
        data = [list(point) for point in dataRaw]

    best_inertia = float('inf')
    best_centroids = None
    best_assignments = None

    for trial in range(repetitions):
        print(f"Starting K-means run {trial + 1}/{repetitions}...")

        current_centroids, current_assignments = kmeans_algorith(data, k)

        inertia = 0.0
        for i, point in enumerate(data):
            inertia+=euclidean_distance(point, current_centroids[current_assignments[i]])**2
        
        if inertia<best_inertia:
            best_inertia=inertia
            best_assignments=current_assignments
            best_centroids=current_centroids
    
    print(f"K-means completed. Best inertia found: {best_inertia:.2f}")
    return best_centroids, best_assignments, best_inertia



def plot_assignment_matrix(true_labels, cluster_assignments, k_clusters, num_digits=10):
    assignment_matrix = np.zeros((num_digits, k_clusters))

    for i in range(len(true_labels)):
        digit = int(true_labels[i])
        cluster = cluster_assignments[i]
        
        if 0 <= digit < num_digits and 0 <= cluster < k_clusters:
            assignment_matrix[digit, cluster] += 1

    row_sums = assignment_matrix.sum(axis=1, keepdims=True)
    assignment_matrix_percent = np.divide(assignment_matrix, row_sums, out=np.zeros_like(assignment_matrix), where=row_sums!=0)
    
    plt.figure(figsize=(k_clusters * 1.2, num_digits * 0.8)) 
    
    try:
        sns.heatmap(assignment_matrix_percent, annot=True, fmt=".1%", cmap="YlGnBu",
                    xticklabels=[f"Cluster {i}" for i in range(k_clusters)],
                    yticklabels=[str(i) for i in range(num_digits)],
                    linewidths=.5, cbar_kws={'label': 'Percentage'})
    except NameError:
        plt.imshow(assignment_matrix_percent, cmap="YlGnBu", aspect="auto")
        plt.colorbar(label='Percentage')
        plt.xticks(np.arange(k_clusters), [f"Cluster {i}" for i in range(k_clusters)], rotation=45, ha='right')
        plt.yticks(np.arange(num_digits), [str(i) for i in range(num_digits)])
        for y in range(num_digits):
            for x in range(k_clusters):
                plt.text(x, y, f"{assignment_matrix_percent[y, x]:.1%}",
                         ha='center', va='center', color='black' if assignment_matrix_percent[y, x] < 0.5 else 'white')

    plt.xlabel("Assigned Cluster")
    plt.ylabel("True Digit")
    plt.title("Percentage Assignment of True Digits to Clusters")
    plt.tight_layout()
    plt.show()

def plot_centroids(centroids, image_shape=(28, 28)):
    num_centroids = len(centroids)
    grid_size = int(np.ceil(math.sqrt(num_centroids)))
    
    plt.figure(figsize=(grid_size * 2, grid_size * 2))
    for i, centroid_vec in enumerate(centroids):
        plt.subplot(grid_size, grid_size, i + 1)
        centroid_image = np.array(centroid_vec).reshape(image_shape)
        plt.imshow(centroid_image, cmap='gray')
        plt.title(f"Centroid {i}")
        plt.axis('off') 
    plt.tight_layout()
    plt.show()

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


    # # Task 1.1: K-means for 10 clusters
    # print("\n--- Running K-means for 10 clusters ---")
    # k_10_centroids, k_10_assignments, k_10_inertia = run_experiment(10, 3, data_points)
    # print(f"K=10: Final Inertia = {k_10_inertia:.2f}")

    # # Task 1.2: Plot assignment matrix for 10 clusters
    # print("\n--- Plotting assignment matrix for 10 clusters ---")
    # plot_assignment_matrix(true_labels_list, k_10_assignments, k_clusters=10)

    # # Task 1.3: Plot centroids for 10 clusters
    # print("\n--- Plotting centroids for 10 clusters ---")
    # plot_centroids(k_10_centroids, image_shape=(28, 28))


    # # Task 1.4: Repeat for 15, 20, and 30 clusters
    # # K=15
    # print("\n--- Running K-means for 15 clusters ---")
    # k_15_centroids, k_15_assignments, k_15_inertia = run_experiment(15, 3, data_points)
    # print(f"K=15: Final Inertia = {k_15_inertia:.2f}")
    # plot_assignment_matrix(true_labels_list, k_15_assignments, k_clusters=15)
    # plot_centroids(k_15_centroids, image_shape=(28, 28))

    # # K=20
    # print("\n--- Running K-means for 20 clusters ---")
    # k_20_centroids, k_20_assignments, k_20_inertia = run_experiment(20, 3, data_points)
    # print(f"K=20: Final Inertia = {k_20_inertia:.2f}")
    # plot_assignment_matrix(true_labels_list, k_20_assignments, k_clusters=20)
    # plot_centroids(k_20_centroids, image_shape=(28, 28))

    print("\n--- Running K-means for 30 clusters ---")
    
    k_30_centroids, k_30_assignments, k_30_inertia =run_experiment(30, 3, data_points)
    print(f"K=30: Final Inertia = {k_30_inertia:.2f}")
    plot_assignment_matrix(true_labels_list, k_30_assignments, k_clusters=30)
    plot_centroids(k_30_centroids, image_shape=(28, 28))