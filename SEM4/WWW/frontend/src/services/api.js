const API_BASE_URL = 'http://localhost:8080'; 

const api = {
   
    getAuthHeaders: (token) => ({
        'Content-Type': 'application/json',
        ...(token && { 'Authorization': `Bearer ${token}` })
    }),

    async fetchWrapper(url, options = {}) {

        const token = options.token; 
        
        const headers = api.getAuthHeaders(token);

        const response = await fetch(`${API_BASE_URL}${url}`, {
            ...options,
            headers: {
                ...headers,
                ...options.headers, 
            },
        });

        if (!response.ok) {
            const errorData = await response.json().catch(() => ({ message: 'Something went wrong' }));
            throw new Error(errorData.message || 'API request failed');
        }
        
        if (response.status === 204) {
            return null;
        }
        return response.json();
    },

    async login(username, password) {
        const response = await fetch(`${API_BASE_URL}/auth/signin`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ username, password }),
        });
        if (!response.ok) {
            const errorData = await response.json().catch(() => ({ message: 'Login failed' }));
            throw new Error(errorData.message || 'Login failed');
        }
        return response.json();
    },

    async register(userData) {
        const response = await fetch(`${API_BASE_URL}/auth/signup`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(userData),
        });
        if (!response.ok) {
            const errorData = await response.text().catch(() => 'Registration failed');
            throw new Error(errorData || 'Registration failed');
        }
        return response.text();
    },

    async getAllGames(token, params) {
        const query = new URLSearchParams(params).toString();
        return await api.fetchWrapper(`/games?${query}`, { method: 'GET', token });
    },

    async getGameById(token, id) {
        return await api.fetchWrapper(`/games/${id}`, { method: 'GET', token });
    },

    async createGame(token, gameData) {
        return await api.fetchWrapper('/games', { method: 'POST', body: JSON.stringify(gameData), token });
    },

    async updateGame(token, id, gameData) {
        return await api.fetchWrapper(`/games/${id}`, { method: 'PUT', body: JSON.stringify(gameData), token });
    },

    async deleteGame(token, id) {
        return await api.fetchWrapper(`/games/${id}`, { method: 'DELETE', token });
    },


    async getAllReviews(token) {
        return await api.fetchWrapper('/reviews', { method: 'GET', token });
    },
    async deleteReview(token, id) {
        return await api.fetchWrapper(`/reviews/${id}`, { method: 'DELETE', token });
    },
    async fetchReviewsByGame(token, gameId) {
        return await api.fetchWrapper(`/reviews/by-game/${gameId}`, { method: 'GET', token });
    },
    async fetchReviewById(token, id) {
        return await api.fetchWrapper(`/reviews/${id}`, { method: 'GET', token });
    },
    async createReview(token, authorId, gameId, reviewDetails) {
        return await api.fetchWrapper(`/reviews/user/${authorId}/game/${gameId}`, { method: 'POST', body: JSON.stringify(reviewDetails), token });
    },
    async updateReview(token, id, updatedReviewDetails) {
        return await api.fetchWrapper(`/reviews/${id}`, { method: 'PUT', body: JSON.stringify(updatedReviewDetails), token });
    },

    async createAdminUser(token, userData) {
        return await api.fetchWrapper('/auth/admin/create', { method: 'POST', body: JSON.stringify(userData), token });
    },
    async deleteUser(token, userId) {
        return await api.fetchWrapper(`/auth/admin/delete/${userId}`, { method: 'DELETE', token });
    },
    async getAllUsers(token) {
        return await api.fetchWrapper('/users', { method: 'GET', token });
    },
};

export { api }; // EXPORT THE 'api' OBJECT