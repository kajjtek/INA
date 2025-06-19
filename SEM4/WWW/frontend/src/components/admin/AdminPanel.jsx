import React, { useContext,createContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext'; // <-- No curly braces for default export
import { Link, useNavigate } from 'react-router-dom';

const AdminPanel = () => {
    const { jwtToken, isAdmin } = useContext(AuthContext);
    const [users, setUsers] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState('');

    const [newAdminUsername, setNewAdminUsername] = useState('');
    const [newAdminPassword, setNewAdminPassword] = useState('');
    const [createAdminError, setCreateAdminError] = useState('');
    const [createAdminSuccess, setCreateAdminSuccess] = useState('');

    const fetchUsers = async () => {
        setLoading(true);
        setError('');
        try {
            // Assuming you add an API endpoint for getting all users accessible by admin
            // For now, this call is hypothetical until a /users GET endpoint is defined in UserController
            const data = await api.fetchWrapper('/users', { method: 'GET', token: jwtToken });
            setUsers(data);
        } catch (err) {
            setError(err.message || 'Failed to fetch users.');
        } finally {
            setLoading(false);
        }
    };

    const handleCreateAdmin = async (e) => {
        e.preventDefault();
        setCreateAdminError('');
        setCreateAdminSuccess('');
        try {
            await api.fetchWrapper('/auth/admin/create', {
                method: 'POST',
                headers: api.getAuthHeaders(jwtToken),
                body: JSON.stringify({ username: newAdminUsername, password: newAdminPassword, roles: ["ROLE_ADMIN"] })
            });
            setCreateAdminSuccess('Admin user created successfully!');
            setNewAdminUsername('');
            setNewAdminPassword('');
            fetchUsers(); // Refresh user list
        } catch (err) {
            setCreateAdminError(err.message || 'Failed to create admin user.');
        }
    };

    const handleDeleteUser = async (userId) => {
        if (window.confirm(`Are you sure you want to delete user with ID: ${userId}?`)) {
            try {
                await api.fetchWrapper(`/auth/admin/delete/${userId}`, { method: 'DELETE', token: jwtToken });
                fetchUsers(); // Refresh user list
            } catch (err) {
                setError(err.message || 'Failed to delete user.');
            }
        }
    };

    useEffect(() => {
        if (isAdmin) {
            fetchUsers();
        }
    }, [isAdmin, jwtToken]);

    if (!isAdmin) return <div className="text-center p-8 text-red-500">Access Denied: Only administrators can view this page.</div>;
    if (loading) return <div className="text-center p-8">Loading admin panel...</div>;
    if (error) return <div className="text-center p-8 text-red-500">Error: {error}</div>;

    return (
        <div className="container mx-auto p-8">
            <h2 className="text-3xl font-bold mb-6 text-center">Admin Panel</h2>

            {/* Create Admin User Section */}
            <div className="bg-white p-8 rounded-lg shadow-lg mb-8">
                <h3 className="text-2xl font-bold mb-4">Create New Admin User</h3>
                <form onSubmit={handleCreateAdmin}>
                    {createAdminError && <p className="text-red-500 mb-4">{createAdminError}</p>}
                    {createAdminSuccess && <p className="text-green-500 mb-4">{createAdminSuccess}</p>}
                    <div className="mb-4">
                        <label className="block text-gray-700 text-sm font-bold mb-2" htmlFor="newAdminUsername">
                            Username
                        </label>
                        <input
                            type="text"
                            id="newAdminUsername"
                            className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 leading-tight focus:outline-none focus:shadow-outline"
                            value={newAdminUsername}
                            onChange={(e) => setNewAdminUsername(e.target.value)}
                            required
                        />
                    </div>
                    <div className="mb-6">
                        <label className="block text-gray-700 text-sm font-bold mb-2" htmlFor="newAdminPassword">
                            Password
                        </label>
                        <input
                            type="password"
                            id="newAdminPassword"
                            className="shadow appearance-none border rounded w-full py-2 px-3 text-gray-700 mb-3 leading-tight focus:outline-none focus:shadow-outline"
                            value={newAdminPassword}
                            onChange={(e) => setNewAdminPassword(e.target.value)}
                            required
                        />
                    </div>
                    <button
                        type="submit"
                        className="bg-purple-500 hover:bg-purple-700 text-white font-bold py-2 px-4 rounded-full focus:outline-none focus:shadow-outline transition-colors"
                    >
                        Create Admin
                    </button>
                </form>
            </div>

            {/* User List Section (Hypothetical: assumes a GET /users endpoint is created for Admin) */}
            <div className="bg-white p-8 rounded-lg shadow-lg">
                <h3 className="text-2xl font-bold mb-4">Manage Users</h3>
                {users.length === 0 ? (
                    <p className="text-gray-600">No users found or endpoint not available.</p>
                ) : (
                    <table className="min-w-full bg-white rounded-lg overflow-hidden">
                        <thead className="bg-gray-200">
                            <tr>
                                <th className="py-3 px-4 text-left text-sm font-semibold text-gray-700">ID</th>
                                <th className="py-3 px-4 text-left text-sm font-semibold text-gray-700">Username</th>
                                <th className="py-3 px-4 text-left text-sm font-semibold text-gray-700">Roles</th>
                                <th className="py-3 px-4 text-left text-sm font-semibold text-gray-700">Actions</th>
                            </tr>
                        </thead>
                        <tbody>
                            {users.map(user => (
                                <tr key={user.id} className="border-b last:border-b-0 hover:bg-gray-50">
                                    <td className="py-3 px-4 text-gray-700">{user.id}</td>
                                    <td className="py-3 px-4 text-gray-700">{user.username}</td>
                                    <td className="py-3 px-4 text-gray-700">{user.roles ? user.roles.join(', ') : 'N/A'}</td>
                                    <td className="py-3 px-4">
                                        <button
                                            onClick={() => handleDeleteUser(user.id)}
                                            className="bg-red-500 hover:bg-red-700 text-white text-sm font-bold py-1 px-3 rounded-full transition-colors"
                                            disabled={user.roles.includes('ROLE_ADMIN')} // Prevent deleting admins from here, or self
                                        >
                                            Delete
                                        </button>
                                    </td>
                                </tr>
                            ))}
                        </tbody>
                    </table>
                )}
            </div>
        </div>
    );
};

export default AdminPanel;