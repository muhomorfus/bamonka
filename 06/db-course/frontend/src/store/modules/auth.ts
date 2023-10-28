import Vuex from 'vuex';
import Cookies from 'cookies-ts';

import * as openapi from '@/openapi';

import API from '@/api';

export default {
    actions: {
        async login(ctx: any, credentials: {login: string, password: string}) {
            try {
                const resp = await API.login({
                    login: credentials.login,
                    password: credentials.password,
                })

                ctx.commit('saveToken', resp.data.token)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async getMe(ctx: any) {
            try {
                const resp = await API.meGet()

                ctx.commit('saveMe', resp.data)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async logout(ctx: any) {
            ctx.commit('dropToken')
        }
    },

    mutations: {
        saveToken(state: any, token: string) {
            const cookies = new Cookies()
            cookies.set('ACCESS_TOKEN', token)
            window.location.replace('/')
        },

        saveMe(state: any, emp: openapi.Employee) {
            state.me = emp
            state.authorized = true
        },

        dropToken(state: any) {
            const cookies = new Cookies()

            cookies.remove('ACCESS_TOKEN')
            state.me = {}
            state.authorized = false

            window.location.replace('/login')
        },
    },

    state: {
        me: {} as openapi.Employee,
        authorized: false,
    },

    getters: {
        me(state: any) {
            return state.me;
        },

        isHR(state: any) {
            return state.me.level == 'hr'
        },

        isAdmin(state: any) {
            return state.me.level == 'admin'
        },

        authorized(state: any) {
            return state.authorized
        }
    },
};