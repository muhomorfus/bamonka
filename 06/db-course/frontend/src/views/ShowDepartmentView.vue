<template>
  <main class="container">
      <br>

    <div class="row">
      <div class="col-md-2"></div>
      <div class="col-md-8">

        <div class="card mb-3">
          <div class="card-body">
            <BreadCrumbs :breadcrumbs="breadcrumbs"/>

            <h1>{{ department.name }}</h1>

            <p class="text-muted mb-0">{{ department.description }}</p>
          </div>
        </div>

        <ProfileShort v-for="m in departmentMembers" :key="m.id" :profile="{'name': m.name, 'nickname': m.nickname, 'working': m.working}"/>

      </div>
      <div class="col-md-2"></div>
    </div>
  </main>
</template>

<script lang="ts">
import {defineComponent} from "vue";
import {mapActions, mapGetters} from "vuex";
import ProfileShort from "@/components/ProfileShort.vue";
import BreadCrumbs from "@/components/BreadCrumbs.vue";

export default defineComponent({
  components: {BreadCrumbs, ProfileShort},
  methods: {
    ...mapActions(['loadDepartment', 'loadDepartmentMembers']),
  },
  created() {
    this.loadDepartment(this.$route.params.id.toString())
    this.loadDepartmentMembers(this.$route.params.id.toString())
  },
  computed: {
    ...mapGetters(['department', 'departmentMembers', 'breadcrumbs']),
  },
});
</script>