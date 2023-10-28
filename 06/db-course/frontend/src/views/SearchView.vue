<template>
  <main class="container">
      <br>
    <div class="row">
      <div class="col-md-3"></div>
      <div v-if="!isQuery" class="col-md-6">
        <h3>Type you search query...</h3>
      </div>
      <div v-else class="col-md-6">
        <h3>Search results for «{{ $route.query.query }}»</h3>

        <div v-if="!searchDepartments && !searchEmployee && !searchTeams">
          No results...
        </div>

        <div v-if="searchEmployee">
          <h5 class="mt-4">Employee</h5>

          <ProfileShort v-for="e in searchEmployee" :key="e.id" :profile="e"/>
        </div>

        <div v-if="searchDepartments">
          <h5 class="mt-4">Departments</h5>

          <div v-for="d in searchDepartments" :key="d.id" class="card mb-2">
            <div class="card-body">
              <h5 class="card-title">{{ d.name }}</h5>

              <a :href="`/departments/${d.id}`" class="card-link stretched-link">Go to department</a>
            </div>
          </div>
        </div>

        <div v-if="searchTeams">
          <h5 class="mt-4">Teams</h5>

          <div v-for="t in searchTeams" :key="t.id" class="card mb-2">
            <div class="card-body">
              <h5 class="card-title">{{ t.name }}</h5>

              <a :href="`/teams/${t.id}`" class="card-link stretched-link">Go to team</a>
            </div>
          </div>
        </div>

      </div>
      <div class="col-md-3"></div>
    </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";
import ProfileShort from "@/components/ProfileShort.vue";

export default defineComponent({
  components: {ProfileShort},
  data() {
    return {
      isQuery: true,
    }
  },
  methods: {
    ...mapActions(['search']),
  },
  created() {
    if (!this.$route.query.query) {
      this.isQuery = false
    } else {
      this.search(this.$route.query.query)
    }
  },
  computed: {
    ...mapGetters(['searchTeams', 'searchEmployee', 'searchDepartments']),
  },
});
</script>