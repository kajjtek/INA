import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';

const ReviewDetail = () => {
    const { id } = useParams();
    const { jwtToken, isAdmin, currentUser } = useContext(AuthContext);
    const [review, setReview] = useState(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState('');
    const navigate = useNavigate();

    useEffect(() => {
        const fetchReview = async () => {
            setLoading(true);
            setError('');
            try {
                const data = await api.fetchWrapper(`/reviews/${id}`, { method: 'GET', token: jwtToken });
                setReview(data);
            } catch (err) {
                setError(err.message || 'Failed to fetch review details.');
            } finally {
                setLoading(false);
            }
        };
        fetchReview();
    }, [id, jwtToken]);

    const handleDelete = async () => {
        if (window.confirm('Are you sure you want to delete this review?')) {
            try {
                await api.deleteReview(jwtToken, id); // Assuming you add deleteReview to api.js
                navigate(-1); // Go back to previous page (e.g., game detail)
            } catch (err) {
                setError(err.message || 'Failed to delete review.');
            }
        }
    };

    const isAuthor = currentUser && review && review.author && currentUser.id === review.author.id;

    if (loading) return <div className="text-center p-8">Loading review details...</div>;
    if (error) return <div className="text-center p-8 text-red-500">Error: {error}</div>;
    if (!review) return <div className="text-center p-8 text-gray-600">Review not found.</div>;

    return (
        <div className="container mx-auto p-8 max-w-2xl">
            <div className="bg-white p-8 rounded-lg shadow-lg">
                <h2 className="text-3xl font-bold mb-4 text-gray-800">Review Details</h2>
                <p className="text-lg text-gray-700 mb-2"><strong>Rating:</strong> {review.rating}</p>
                <p className="text-gray-700 mb-4"><strong>Description:</strong> {review.description}</p>
                <p className="text-gray-600 text-sm mb-2"><strong>Author:</strong> {review.author ? review.author.username : 'N/A'}</p>
                <p className="text-gray-600 text-sm mb-4"><strong>Game:</strong> {review.game ? review.game.name : 'N/A'}</p>

                {(isAdmin || isAuthor) && (
                    <div className="mt-4 flex space-x-4">
                        {isAuthor && (
                            <Link
                                to={`/reviews/edit/${review.id}`}
                                className="bg-yellow-500 hover:bg-yellow-700 text-white font-bold py-2 px-4 rounded-full transition-colors"
                            >
                                Edit Review
                            </Link>
                        )}
                        {isAdmin && (
                            <button
                                onClick={handleDelete}
                                className="bg-red-500 hover:bg-red-700 text-white font-bold py-2 px-4 rounded-full transition-colors"
                            >
                                Delete Review
                            </button>
                        )}
                    </div>
                )}
            </div>
        </div>
    );
};

export default ReviewDetail;