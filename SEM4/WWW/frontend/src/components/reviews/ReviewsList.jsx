import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';

const ReviewsList = () => {
    const { jwtToken, isAdmin, currentUser } = useContext(AuthContext);
    const [reviews, setReviews] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState('');
    const navigate = useNavigate();

    const fetchReviews = async () => {
        setLoading(true);
        setError('');
        try {
            const data = await api.getAllReviews(jwtToken);
            setReviews(data);
        } catch (err) {
            setError(err.message || 'Failed to fetch reviews.');
        } finally {
            setLoading(false);
        }
    };

    useEffect(() => {
        fetchReviews();
    }, [jwtToken]);

    const handleDelete = async (id) => {
        if (window.confirm('Are you sure you want to delete this review?')) {
            try {
                await api.deleteReview(jwtToken, id);
                fetchReviews(); // Refresh list
            } catch (err) {
                setError(err.message || 'Failed to delete review.');
            }
        }
    };

    if (loading) return <div className="text-center p-8">Loading reviews...</div>;
    if (error) return <div className="text-center p-8 text-red-500">Error: {error}</div>;

    return (
        <div className="container mx-auto p-8">
            <h2 className="text-3xl font-bold mb-6 text-center">All Reviews</h2>
            {reviews.length === 0 ? (
                <p className="text-center text-gray-600">No reviews found.</p>
            ) : (
                <div className="grid grid-cols-1 md:grid-cols-2 lg:grid-cols-3 gap-6">
                    {reviews.map(review => (
                        <div key={review.id} className="bg-white rounded-lg shadow-md p-6">
                            <h3 className="text-xl font-semibold mb-2 text-blue-800">Rating: {review.rating}</h3>
                            <p className="text-gray-700 mb-2 truncate">{review.description}</p>
                            <p className="text-gray-600 text-sm">By: {review.author ? review.author.username : 'Unknown User'}</p>
                            <p className="text-gray-600 text-sm mb-4">For Game: {review.game ? review.game.name : 'Unknown Game'}</p>
                            <Link to={`/reviews/${review.id}`} className="bg-blue-500 hover:bg-blue-700 text-white text-sm font-bold py-2 px-4 rounded-full mr-2 transition-colors">
                                View Details
                            </Link>
                            {(isAdmin || (currentUser && review.author && currentUser.id === review.author.id)) && (
                                <Link
                                    to={`/reviews/edit/${review.id}`}
                                    className="bg-yellow-500 hover:bg-yellow-700 text-white text-sm font-bold py-2 px-4 rounded-full mr-2 transition-colors"
                                >
                                    Edit
                                </Link>
                            )}
                            {isAdmin && (
                                <button
                                    onClick={() => handleDelete(review.id)}
                                    className="bg-red-500 hover:bg-red-700 text-white text-sm font-bold py-2 px-4 rounded-full transition-colors"
                                >
                                    Delete
                                </button>
                            )}
                        </div>
                    ))}
                </div>
            )}
        </div>
    );
};

export default ReviewsList;