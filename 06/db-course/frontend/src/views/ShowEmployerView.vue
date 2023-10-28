<template>
  <main class="container">
    <br>

    <div class="row">
      <div class="col-md-2"></div>
      <div class="col-md-8">

        <div class="card">
          <div class="card-body">
            <BreadCrumbs :breadcrumbs="breadcrumbs" :last="employer.name"/>

            <div v-if="!employer.working" class="alert alert-dark small">
              This employee is deactivated.
            </div>

            <div v-if="nearest" class="alert alert-success small">
              <b>Vacation from {{ new Date(nearest.start).toISOString().slice(0, 10) }} to {{ new Date(nearest.end).toISOString().slice(0, 10) }}</b>: {{ nearest.description }}
            </div>

            <div class="row">
              <div class="col-md-4">
                <img class="w-100 mb-2" style="aspect-ratio: 1" :src="employer.photo" alt="">

                <div v-if="employer.nickname !== me.nickname">
                  <button v-if="!subscribed" class="btn btn-primary w-100" @click="triggerSubscription">Subscribe</button>
                  <button v-else class="btn btn-outline-primary w-100" @click="triggerSubscription">Unsubscribe</button>
                </div>

                <a v-if="employer.nickname === me.nickname" class="btn btn-primary w-100" :href="`/employee/${me.nickname}/edit`">Edit</a>
              </div>
              <div class="col-md-8">
                <h3>
                  {{ employer.name }}
                  <span class="text-muted me-2">@{{ employer.nickname }}</span>
                </h3>
                <h5 class="text-muted">
                  {{ employer.position }} <b v-if="employer.level === 'admin'">(Admin)</b>
                </h5>

                <p class="mt-4">
                  {{ employer.description }}
                </p>

                <p class="mt-3 mb-0"><b>Phone: </b>{{ employer.phone }}</p>
                <p class="mt-0 mb-0"><b>Mail: </b>{{ employer.email }}</p>
                <p class="mt-0 mb-0" v-for="(val, key) in employer.socials" :key="key"><b>{{ key }}: </b>{{ val }}</p>

                <p class="mt-4 text-muted">
                  In organization from {{ new Date(employer.created).toISOString().slice(0, 10) }}.
                </p>

                <ul v-if="employer.teams" class="list-group mt-4">
                  <li v-for="t in employer.teams" :key="t.team.id" class="list-group-item">
                    {{ t.role }} / <a :href="`/teams/${t.team.id}`">{{ t.team.name }}</a>
                  </li>
                </ul>
              </div>
            </div>
          </div>
        </div>

      </div>
      <div class="col-md-2"></div>
    </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";
import BreadCrumbs from "@/components/BreadCrumbs.vue";

export default defineComponent({
  components: {BreadCrumbs},
  methods: {
    ...mapActions(['loadEmployer', 'triggerSubscription', 'loadVacations']),
  },
  created() {
    this.loadEmployer(this.$route.params.nickname.toString()).then(() => {
      this.loadVacations(this.employer.id)
    })

  },
  computed: {
    ...mapGetters(['employer', 'breadcrumbs', 'subscribed', 'me', 'nearest']),
  },
});
</script>