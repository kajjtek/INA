import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';

const ReviewForm = () => {
    const { id } = useParams(); // For edit mode
    const [searchParams] = useSearchParams(); // For creating new review for a specific game
    const gameIdFromUrl = searchParams.get('gameId');

    const { jwtToken, currentUser } = useContext(AuthContext);
    const navigate = useNavigate();
    const [review, setReview] = useState({ rating: 'Good', description: '' });
    const [loading, setLoading] = useState(false);
    const [error, setError] = useState('');
    const [isEditing, setIsEditing] = useState(false);

    const ratings = ['Goated', 'Excellent', 'Good', 'Average', 'Bad', 'Horrible']; // From Rating enum

    useEffect(() => {
        if (id) {
            setIsEditing(true);
            const fetchReview = async () => {
                setLoading(true);
                try {
                    const data = await api.fetchWrapper(`/reviews/${id}`, { method: 'GET', token: jwtToken });
                    setReview(data);
                } catch (err) {
                    setError(err.message || 'Failed to fetch review for editing.');
                } finally {
                    setLoading(false);
                }
            };
            fetchReview();
        }
    }, [id, jwtToken]);

    const handleChange = (e) => {
        setReview({ ...review, [e.target.name]: e.target.value });
    };

    const handleSubmit = async (e) => {
        e.preventDefault();
        setLoading(true);
        setError('');

        if (!currentUser) {
            setError("You must be logged in to create/edit reviews.");
            setLoading(false);
            return;
        }

        try {
            if (isEditing) {
                await api.fetchWrapper(`/reviews/${id}`, {
                    method: 'PUT',
                    body: JSON.stringify(review),
                    token: jwtToken
                });
            } else {
                if (!gameIdFromUrl) {
                    setError("Game ID is required to create a new review.");
                    setLoading(false);
                    return;
                }
                await api.fetchWrapper(`/reviews/user/${currentUser.id}/game/${gameIdFromUrl}`, {
                    method: 'POST',
                    body: JSON.stringify(review),
                    token: jwtToken
                });
            }
            navigate(isEditing ? `/reviews/${id}` : `/games/${gameIdFromUrl}`); // Redirect
        } catch (err) {
            setError(err.message || 'Failed to save review.');
        } finally {
            setLoading(false);
        }
    };

    if (loading && isEditing) return <div className="text-center p-8">Loading review data...</div>;
    if (error) return <div className="text-center p-8 text-red-500">Error: {error}</div>;

    return (
        <div className="container mx-auto p-8 max-w-md">
            <h2 className="text-3xl font-bold mb-6 text-center">{isEditing ? 'Edit Review' : 'Add New Review'}</h2>
            <form onSubmit={handleSubmit} className="bg-white p-8 rounded-lg shadow-lg">
                <div className="mb-4">
                    <label className="block text-gray-700 text-sm font-bold mb-2" htmlFor="rating">
                        Rating
                    </label>
                    <select
                        id="rating"
                        name="rating"
                        value={review.rating}
                        onChange={handleChange}
                        className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
                        required
                    >
                        {ratings.map(r => <option key={r} value={r}>{r}</option>)}
                    </select>
                </div>
                <div className="mb-6">
                    <label className="block text-gray-700 text-sm font-bold mb-2" htmlFor="description">
                        Description
                    </label>
                    <textarea
                        id="description"
                        name="description"
                        value={review.description}
                        onChange={handleChange}
                        className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
                        rows="4"
                        required
                    ></textarea>
                </div>
                <div className="flex items-center justify-between">
                    <button
                        type="submit"
                        className="bg-blue-500 hover:bg-blue-700 text-white font-bold py-2 px-4 rounded-full focus:outline-none focus:shadow-outline transition-colors"
                        disabled={loading}
                    >
                        {loading ? 'Saving...' : (isEditing ? 'Update Review' : 'Add Review')}
                    </button>
                    <button
                        type="button"
                        onClick={() => navigate(-1)} // Go back to previous page
                        className="bg-gray-500 hover:bg-gray-700 text-white font-bold py-2 px-4 rounded-full focus:outline-none focus:shadow-outline transition-colors"
                    >
                        Cancel
                    </button>
                </div>
            </form>
        </div>
    );
};

export default ReviewForm;