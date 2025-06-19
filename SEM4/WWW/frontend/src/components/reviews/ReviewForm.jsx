import React, { useContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate, useParams, useSearchParams} from 'react-router-dom';
import { api } from '../../services/api';

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
                    const data = await api.fetchReviewById(jwtToken, id);
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
                await api.updateReview(jwtToken, id, review);
            } else {
                if (!gameIdFromUrl) {
                    setError("Game ID is required to create a new review.");
                    setLoading(false);
                    return;
                }
                await api.createReview(jwtToken, currentUser.id, gameIdFromUrl, review);
            }
            navigate(isEditing ? `/reviews/${id}` : `/games/${gameIdFromUrl}`); // Redirect
        } catch (err) {
            setError(err.message || 'Failed to save review.');
        } finally {
            setLoading(false);
        }
    };

    if (loading && isEditing) return <div className="text-center p-8 text-xl font-semibold">Loading review data...</div>;
    if (error) return <div className="text-center p-8 text-red-500 text-xl font-semibold">Error: {error}</div>;

    return (
        <div className="container mx-auto p-8 min-h-[calc(100vh-80px)] flex items-center justify-center">
            <div className="bg-white p-8 md:p-10 rounded-xl shadow-2xl max-w-md w-full border border-purple-200">
                <h2 className="text-4xl font-bold mb-8 text-center text-gray-800">{isEditing ? 'Edit Review' : 'Add New Review'}</h2>
                <form onSubmit={handleSubmit} className="space-y-6">
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="rating">
                            Rating
                        </label>
                        <select
                            id="rating"
                            name="rating"
                            value={review.rating}
                            onChange={handleChange}
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent transition duration-200"
                            required
                        >
                            {ratings.map(r => <option key={r} value={r}>{r}</option>)}
                        </select>
                    </div>
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="description">
                            Description
                        </label>
                        <textarea
                            id="description"
                            name="description"
                            value={review.description}
                            onChange={handleChange}
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent transition duration-200"
                            rows="4"
                            required
                        ></textarea>
                    </div>
                    <div className="flex flex-col sm:flex-row items-center justify-center space-y-4 sm:space-y-0 sm:space-x-4 mt-6">
                        <button
                            type="submit"
                            className="bg-purple-600 hover:bg-purple-700 text-white font-bold py-2.5 px-6 rounded-full focus:outline-none focus:shadow-outline transition-all duration-300 transform hover:scale-105 shadow-lg w-full sm:w-auto"
                            disabled={loading}
                        >
                            {loading ? 'Saving...' : (isEditing ? 'Update Review' : 'Add Review')}
                        </button>
                        <button
                            type="button"
                            onClick={() => navigate(-1)} // Go back to previous page
                            className="bg-gray-500 hover:bg-gray-600 text-white font-bold py-2.5 px-6 rounded-full focus:outline-none focus:shadow-outline transition-all duration-300 transform hover:scale-105 shadow-lg w-full sm:w-auto"
                        >
                            Cancel
                        </button>
                    </div>
                </form>
            </div>
        </div>
    );
};

export default ReviewForm;