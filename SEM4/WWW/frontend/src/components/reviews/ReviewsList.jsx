import React, { useContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';
import { api } from '../../services/api';

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
            // Brak parametrów, aby pobrać wszystkie recenzje
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
    }, [jwtToken]); // Ponowne pobieranie po zmianie tokena JWT

    const handleDelete = async (id) => {
        if (window.confirm('Are you sure you want to delete this review?')) {
            try {
                await api.deleteReview(jwtToken, id);
                fetchReviews(); // Odśwież listę po usunięciu
            } catch (err) {
                setError(err.message || 'Failed to delete review.');
            }
        }
    };

    if (loading) return <div className="text-center p-8 text-xl font-semibold text-gray-700">Loading reviews...</div>;
    if (error) return <div className="text-center p-8 text-red-600 text-xl font-semibold">Error: {error}</div>;

    return (
        <div className="container mx-auto px-4 sm:px-6 lg:px-8 max-w-full"> {/* Zmieniono z p-8 na px-4 sm:px-6 lg:px-8 i max-w-full */}
            <h2 className="text-4xl font-bold mb-8 text-center text-white-800">All Reviews</h2> {/* Upewniono się, że tytuł jest widoczny i centralny */}

            {reviews.length === 0 ? (
                <p className="text-center text-gray-600 text-xl">No reviews found.</p>
            ) : (
                <div className="grid grid-cols-1 sm:grid-cols-2 lg:grid-cols-3 xl:grid-cols-4 gap-6"> {/* Więcej kolumn na dużych ekranach */}
                    {reviews.map(review => (
                        <div key={review.id} className="bg-white rounded-xl shadow-lg p-6 transform hover:scale-105 transition-transform duration-300 border border-gray-200">
                            <h3 className="text-2xl font-bold mb-2 text-blue-800 leading-tight">Rating: {review.rating}</h3> {/* Większa i pogrubiona ocena */}
                            <p className="text-gray-700 text-base mb-2 line-clamp-3">{review.description}</p> {/* Ograniczenie tekstu */}
                            <p className="text-gray-600 text-sm">By: <span className="font-medium">{review.author ? review.author.username : 'Unknown User'}</span></p>
                            <p className="text-gray-600 text-sm mb-4">For Game: <span className="font-medium">{review.game ? review.game.name : 'Unknown Game'}</span></p>
                            <div className="flex flex-wrap gap-2 mt-4">
                                <Link to={`/reviews/${review.id}`} className="flex-1 text-center bg-blue-500 hover:bg-blue-600 text-white text-sm font-bold py-2.5 px-4 rounded-full transition-colors duration-200 shadow-md">
                                    View Details
                                </Link>
                                {(isAdmin || (currentUser && review.author && currentUser.id === review.author.id)) && (
                                    <Link
                                        to={`/reviews/edit/${review.id}`}
                                        className="flex-1 text-center bg-yellow-500 hover:bg-yellow-600 text-white text-sm font-bold py-2.5 px-4 rounded-full transition-colors duration-200 shadow-md"
                                    >
                                        Edit
                                    </Link>
                                )}
                                {isAdmin && (
                                    <button
                                        onClick={() => handleDelete(review.id)}
                                        className="flex-1 text-center bg-red-500 hover:bg-red-600 text-white text-sm font-bold py-2.5 px-4 rounded-full transition-colors duration-200 shadow-md"
                                    >
                                        Delete
                                    </button>
                                )}
                            </div>
                        </div>
                    ))}
                </div>
            )}
        </div>
    );
};

export default ReviewsList;