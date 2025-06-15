const API_BASE_URL = 'http://localhost:8080'; // **IMPORTANT: Replace with your actual backend API URL**

const api = {
    // Utility to get authenticated headers
    getAuthHeaders: (token) => ({
        'Content-Type': 'application/json',
        ...(token && { 'Authorization': `Bearer ${token}` })
    }),

    // Generic fetch wrapper
    async fetchWrapper(url, options = {}) {
        const { jwtToken } = useContext(AuthContext); // Access context inside component/hook
        const headers = api.getAuthHeaders(jwtToken);
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
        // Handle 204 No Content for DELETE
        if (response.status === 204) {
            return null;
        }
        return response.json();
    },

    // Example API calls (you would add more for each resource)
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
            const errorData = await response.text().catch(() => 'Registration failed'); // Backend returns plain text for "User already taken"
            throw new Error(errorData || 'Registration failed');
        }
        return response.text(); // Expecting "User registered successfully!"
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
    // ... add more API functions for reviews, users, etc.
};