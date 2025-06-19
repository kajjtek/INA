import React, { useContext, useState, useEffect } from 'react';
import AuthContext from '../../context/AuthContext';
import { Link } from 'react-router-dom';
import { api } from '../../services/api';

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
            const data = await api.getAllUsers(jwtToken);
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
            await api.createAdminUser(jwtToken, { username: newAdminUsername, password: newAdminPassword, roles: ["ROLE_ADMIN"] });
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
                await api.deleteUser(jwtToken, userId);
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

    if (!isAdmin) return <div className="text-center p-8 text-red-500 text-xl font-semibold">Access Denied: Only administrators can view this page.</div>;
    if (loading) return <div className="text-center p-8 text-xl font-semibold">Loading admin panel...</div>;
    if (error) return <div className="text-center p-8 text-red-500 text-xl font-semibold">Error: {error}</div>;

    return (
        <div className="container mx-auto p-8">
            <h2 className="text-4xl font-bold mb-8 text-center text-white-800">Admin Panel</h2>

            {/* Create Admin User Section */}
            <div className="bg-white p-8 rounded-xl shadow-lg mb-8 border border-purple-200">
                <h3 className="text-2xl font-bold mb-5 text-gray-800">Create New Admin User</h3>
                <form onSubmit={handleCreateAdmin} className="space-y-6">
                    {createAdminError && <p className="text-red-500 bg-red-100 p-3 rounded-md mb-4 text-center border border-red-200">{createAdminError}</p>}
                    {createAdminSuccess && <p className="text-green-500 bg-green-100 p-3 rounded-md mb-4 text-center border border-green-200">{createAdminSuccess}</p>}
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="newAdminUsername">
                            Username
                        </label>
                        <input
                            type="text"
                            id="newAdminUsername"
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 leading-tight focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent transition duration-200"
                            value={newAdminUsername}
                            onChange={(e) => setNewAdminUsername(e.target.value)}
                            required
                        />
                    </div>
                    <div className="relative">
                        <label className="block text-gray-700 text-sm font-semibold mb-2" htmlFor="newAdminPassword">
                            Password
                        </label>
                        <input
                            type="password"
                            id="newAdminPassword"
                            className="shadow-sm appearance-none border border-gray-300 rounded-lg w-full py-2.5 px-4 text-gray-700 mb-3 leading-tight focus:outline-none focus:ring-2 focus:ring-purple-500 focus:border-transparent transition duration-200"
                            value={newAdminPassword}
                            onChange={(e) => setNewAdminPassword(e.target.value)}
                            required
                        />
                    </div>
                    <button
                        type="submit"
                        className="bg-purple-600 hover:bg-purple-700 text-white font-bold py-2.5 px-6 rounded-full focus:outline-none focus:shadow-outline transition-all duration-300 transform hover:scale-105 shadow-lg"
                    >
                        Create Admin
                    </button>
                </form>
            </div>

            {/* User List Section (Hypothetical: assumes a GET /users endpoint is created for Admin) */}
            <div className="bg-white p-8 rounded-xl shadow-lg border border-gray-200">
                <h3 className="text-2xl font-bold mb-5 text-gray-800">Manage Users</h3>
                {users.length === 0 ? (
                    <p className="text-gray-600 text-lg">No users found or endpoint not available.</p>
                ) : (
                    <div className="overflow-x-auto"> {/* Responsywna tabela */}
                        <table className="min-w-full bg-white rounded-lg">
                            <thead className="bg-gray-100 border-b border-gray-200">
                                <tr>
                                    <th className="py-3 px-4 text-left text-sm font-semibold text-gray-700 uppercase tracking-wider">ID</th>
                                    <th className="py-3 px-4 text-left text-sm font-semibold text-gray-700 uppercase tracking-wider">Username</th>
                                    <th className="py-3 px-4 text-left text-sm font-semibold text-gray-700 uppercase tracking-wider">Roles</th>
                                    <th className="py-3 px-4 text-left text-sm font-semibold text-gray-700 uppercase tracking-wider">Actions</th>
                                </tr>
                            </thead>
                            <tbody>
                                {users.map(user => (
                                    <tr key={user.id} className="border-b border-gray-100 last:border-b-0 hover:bg-gray-50 transition-colors duration-150">
                                        <td className="py-3 px-4 text-gray-700">{user.id}</td>
                                        <td className="py-3 px-4 text-gray-700 font-medium">{user.username}</td>
                                        <td className="py-3 px-4 text-gray-700">{user.roles ? user.roles.join(', ') : 'N/A'}</td>
                                        <td className="py-3 px-4">
                                            <button
                                                onClick={() => handleDeleteUser(user.id)}
                                                className="bg-red-500 hover:bg-red-600 text-white text-sm font-bold py-1.5 px-4 rounded-full transition-colors duration-200 shadow-md"
                                                disabled={user.roles.includes('ROLE_ADMIN')} // Prevent deleting admins from here, or self
                                            >
                                                Delete
                                            </button>
                                        </td>
                                    </tr>
                                ))}
                            </tbody>
                        </table>
                    </div>
                )}
            </div>
        </div>
    );
};

export default AdminPanel;