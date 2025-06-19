import React, { useContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate, useParams } from 'react-router-dom';
import { api } from '../../services/api';

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
                const data = await api.fetchReviewById(jwtToken, id);
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

    if (loading) return <div className="text-center p-8 text-xl font-semibold">Loading review details...</div>;
    if (error) return <div className="text-center p-8 text-red-500 text-xl font-semibold">Error: {error}</div>;
    if (!review) return <div className="text-center p-8 text-gray-600 text-xl">Review not found.</div>;

    return (
        <div className="container mx-auto p-8 min-h-[calc(100vh-80px)] flex items-center justify-center">
            <div className="bg-white p-8 md:p-10 rounded-xl shadow-2xl max-w-2xl w-full border border-purple-200">
                <h2 className="text-4xl font-bold mb-6 text-gray-800 text-center">Review Details</h2>
                <div className="space-y-3">
                    <p className="text-xl text-gray-800"><strong>Rating:</strong> <span className="font-semibold text-purple-600">{review.rating}</span></p> {/* Wyróżniona ocena */}
                    <p className="text-lg text-gray-700"><strong>Description:</strong> {review.description}</p>
                    <p className="text-md text-gray-600"><strong>Author:</strong> <span className="font-medium text-blue-600">{review.author ? review.author.username : 'N/A'}</span></p>
                    <p className="text-md text-gray-600"><strong>Game:</strong> <span className="font-medium text-blue-600">{review.game ? review.game.name : 'N/A'}</span></p>
                </div>

                {(isAdmin || isAuthor) && (
                    <div className="mt-8 flex flex-wrap gap-3 justify-center">
                        {isAuthor && (
                            <Link
                                to={`/reviews/edit/${review.id}`}
                                className="bg-yellow-500 hover:bg-yellow-600 text-white font-bold py-2.5 px-6 rounded-full transition-all duration-300 transform hover:scale-105 shadow-lg"
                            >
                                Edit Review
                            </Link>
                        )}
                        {isAdmin && (
                            <button
                                onClick={handleDelete}
                                className="bg-red-500 hover:bg-red-600 text-white font-bold py-2.5 px-6 rounded-full transition-all duration-300 transform hover:scale-105 shadow-lg"
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