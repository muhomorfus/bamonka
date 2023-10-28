import Vuex from 'vuex';
import Cookies from 'cookies-ts';

import * as openapi from '@/openapi';

import API from '@/api';

export default {
    actions: {
        async loadVacations(ctx: any, id: string) {
            try {
                const resp = await API.getVacationList(id)
                ctx.commit('saveVacations', resp.data.list)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        },

        async createVacation(ctx: any) {
            try {
                const resp = await API.createVacation(ctx.state.vacation)
                ctx.commit('saveVacation', resp.data)
            } catch (e) {
                console.error(e);

                if (e.response.data && e.response.data.comment) {
                    ctx.dispatch('showError', e.response.data.comment)
                } else {
                    ctx.dispatch('showError', 'Unknown error.')
                }
            }
        }
    },

    mutations: {
        saveVacations(state: any, vacations: Array<openapi.Vacation>) {
            state.vacations = vacations
        },

        saveVacation(state: any, vacation: openapi.Vacation) {
            state.vacations.push(vacation)
        },
    },

    state: {
        vacations: Array<openapi.Vacation>(),
        vacation: {} as openapi.Vacation,
    },

    getters: {
        vacations(state: any) {
            return state.vacations
        },

        vacation(state: any) {
            return state.vacation
        },

        nearest(state: any) {
            if (state.vacations.length) {
                return state.vacations[0]
            }

            return null
        },
    },
};